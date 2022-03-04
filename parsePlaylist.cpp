#include <iostream>
#include <iterator>
#include <fstream>
#include <string>
#include <map>
#include <regex>
#include <algorithm>
#include <vector>
using namespace std;

/**
 * @brief 
 * EXTM3U
 * EXT-X-VERSION
 * EXT-X-SESSION-DATA // DATA-ID* + VALUE|URI, Same*&DiffLANG
 * EXT-X-SESSION-KEY
 * EXT-X-INDEPENDENT-SEGMENTS
 * EXT-X-START
 * 
 * EXT-X-MEDIA
 * EXT-X-STREAM-INF
 * EXT-X-I-FRAME-STREAM-INF
 */

/*
 * instance of each newMedia manifestrendition EXT-X-MEDIA
*/
class mediaRendition {
    map<string, string> attributes;
    string groupID;
    string name;
    string type;
    string uri;
    string tag;

    // func: take in line string & parse for attributes
    public: int Parse(string newLine) {
       
        // parse by "," delimiter from begin to end of newLine
        regex regex("[:|,]"), regex2("=");
        vector<string> out(
                    sregex_token_iterator(newLine.begin(), newLine.end(), regex, -1),
                    sregex_token_iterator()
                    );
 
        // create key:value pairs for each "=" delimiter within
        for (auto &entry: out) {
            int index;
            if ((index = entry.find("#", 0)) != string::npos) {
                attributes["TAG"] = entry;
                tag = entry;

            } else {                            // parse attrs & add to map
                vector<string> output(
                    sregex_token_iterator(entry.begin(), entry.end(), regex2, -1),
                    sregex_token_iterator()
                    );

                if (output.size() == 2) {
                    string arr[output.size()];
                    copy(output.begin(), output.end(), arr);
                    attributes[arr[0]] = arr[1];
                }
            }
        }
        setRequired();
        // cout << "Attritubes for this Media Rendition are set." << endl;
        // displayAttr();
        
        return 0;
   }

    private: int displayAttr() {
        for (auto &x: attributes) {
            cout << x.first    // string (key)
                << ':'
                << x.second   // string's value 
                << endl;
        }
        cout << endl;
        return 0;
    }

    int setRequired() {
        if (!attributes.at("GROUP-ID").empty()) {
            groupID = attributes["GROUP-ID"];
        }

        if (!attributes.at("NAME").empty()) {
            name = attributes["NAME"];
        }

        if (!attributes.at("TYPE").empty()) {
            type = attributes["TYPE"];
        }

        if (!attributes.at("URI").empty()) {
            uri = attributes["URI"];
        }
    }

    // getters for required 
    public: string getGroupID() { return groupID;}
    public: string getName() { return name; }
    public: string getType() { return type; }
    public: string getURI() { return uri; }
    public: string getTag() { return tag; }

};

/*
 * instance of each mediavariant stream 
 * EXT-X-STREAM-INF | EXT-X-I-FRAME-STREAM-INF
*/
class mediaVariant {
    map<string, string> attributes;
    string types[4] = {"AUDIO", "VIDEO", "SUBTITLES", "CAPTIONS"};
    string uri;
    int bandwidth;
    string type;
    string typeID;
    string tag;

    // func: take in line string & parse for attributes
    public: int Parse(string newLine) {
       
        // parse by "," delimiter from begin to end of newLine
        regex regex(",(?=(?:[^\"]*\"[^\"]*\")*[^\"]*$)|:"), regex2("=");
        vector<string> out(
                    sregex_token_iterator(newLine.begin(), newLine.end(), regex, -1),
                    sregex_token_iterator()
                    );
 
        // create key:value pairs for each "=" delimiter withsin
        for (auto &entry: out) {
            int index;
            if ((index = entry.find("#", 0)) != string::npos) {
                attributes["TAG"] = entry;
                tag = entry;

            } else {                            // parse attrs & add to map
                vector<string> output(
                    sregex_token_iterator(entry.begin(), entry.end(), regex2, -1),
                    sregex_token_iterator()
                    );

                if (output.size() == 2) {
                    string arr[output.size()];
                    copy(output.begin(), output.end(), arr);
                    attributes[arr[0]] = arr[1];
                }
            }
        }
        setRequired();
        // cout << "Attritubes for this Media Variant are set." << endl;
        // displayAttr();
        
        return 0;
   }

    private: int displayAttr() {
        for (auto &x: attributes) {
            cout << x.first    // string (key)
                << ':'
                << x.second   // string's value 
                << endl;
        }
        cout << endl;
        return 0;
    }

    int setRequired() {
        if (!attributes.at("URI").empty()) {
            uri = attributes["URI"];
        }

        if (!attributes.at("BANDWIDTH").empty()) {
            bandwidth = stoi(attributes["BANDWIDTH"]);
        }

        setTypeID();
    }

    int setTypeID() {
        for (string key : types) {
            if (attributes.count(key) != 0) {
                type = key;
                typeID = attributes.at(key);
            }
        }
        return 0;
    }

     // getters for required 
    public: int getBandwidth() { return bandwidth; }
    public: string getTypeID() { return typeID; }
    public: string getURI() { return uri; }
    public: string getTag() { return tag; }

};

/*
* read in file 
* parse newMedia instances
* sort by attributes
*/
class masterPlaylist {
    bool isPlaylist;
    bool isValidMaster;
    bool independentSegs;
    // structure to hold all master attr map<String Tag, map<String, String>>
    vector<mediaRendition> renditions;  // structure to hold all newMedia renditions List of <mediaRendition>
    vector<mediaVariant> variants;      // structure to hold all rendition mediavariants List of <mediaVariant>

    // read in file line by line starting with #
    public: int readFile(string filename) {
        ifstream fileReader;
        fileReader.open(filename);
        string nextLine;

        //cout << "Reading from the file" << endl; 
        getline(fileReader, nextLine);
        if (nextLine == "#EXTM3U") {
            isPlaylist = true;              //cout << "is playlist" << endl;
            nextLine = "";
        } else {
            isPlaylist = false;
        }

        // validate Master plylist rules
        while(isPlaylist & fileReader.good()) {
            getline(fileReader, nextLine);
            int pos = 0;
        
            if (!nextLine.empty()) {
                string newMedia = nextLine;
                int index;
                // Use Tag to create & set Rendition or Variant instance
                if ((index = newMedia.find("#EXT-X-MEDIA", 0)) != string::npos) {
                    // set rendition instance
                    mediaRendition media;
                    media.Parse(newMedia);
                    renditions.push_back(media);

                } else if ((index = newMedia.find("#EXT-X-STREAM-INF", 0)) != string::npos) {
                    // get URI
                    string next;
                    getline(fileReader, next);
                    string uri = next;
                    if(uri.find(".m3u8") != string::npos) {
                        newMedia = newMedia + ",URI=" + uri;
                    } 

                    // set variant instance
                    mediaVariant var;
                    var.Parse(newMedia);
                    variants.push_back(var);

                } else if (((index = newMedia.find("#EXT-X-I-FRAME-STREAM-INF", 0)) != string::npos)) {
                    // set variant instance
                    mediaVariant var;
                    var.Parse(newMedia);
                    variants.push_back(var); 

                } else {
                    // global variables
                }
            }
        }
         return 0;
    }

    int getRenditions() {
         for (auto &x: renditions) {
            cout << x.getTag() << " - " << x.getGroupID() << endl;
        }
        return 0;
    }

    int getVariants() {
         for (auto &x: variants) {
            cout << x.getTag() << " - " << x.getTypeID() << ": " << x.getURI() << endl;
        }
        return 0;
    }

    int adapt(int bitrate) {
        // match given bitrate with avaliable variant bandwidth
        for (auto &var: variants) {
            if (var.getBandwidth() == bitrate) {
                for (auto &ren: renditions) {
                    // check groupID
                    if (ren.getGroupID() == var.getTypeID()) {
                             cout << endl << "match found @ " 
                                << bitrate
                                << " mbps: Variant - " 
                                << var.getURI()  
                                << " using Rendition - " 
                                << ren.getGroupID()  
                                << endl << endl;
                    }
                }
            }
        }
    }

};

// main() is where program execution begins.
int main() {
    masterPlaylist MP;               // Create instance of masterPlaylist
    MP.readFile("master.m3u8");      // Set input file
    MP.getRenditions();
    MP.getVariants();
    MP.adapt(3918799);

   return 0;
}
