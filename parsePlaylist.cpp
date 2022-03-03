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
class MediaRendition {
    map<string, string> attributes;
    enum Type {AUDIO, VIDEO, SUBTITLES, CAPTIONS};
    string groupID;
    string name;

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
            if ((index = entry.find("#EXT-X-MEDIA", 0)) != string::npos) {
                attributes["TAG"] = "EXT-X-MEDIA";

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
        cout << "Attritubes for this Media Rendition are set." << endl;
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
    }

    public: string getGroupID(){
            return groupID;

    }




    // getters for required 

};

/*
 * instance of each variant stream 
 * EXT-X-STREAM-INF | EXT-X-I-FRAME-STREAM-INF
*/
class Variant {
    // Hashmap attributes = Hashmap<String, String>;
    string URI;
    int Bandwidth; 
    // if enum TYpe Key present must match some GroupID present

     // func: take in line string & parse for attributes
    // Hashmap Parse(string newLine) {
        // parse by "," delimiter excluding CODECS ","
        // create key:value pairs for each "=" within

   // }


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
    // structure to hold all master attr Hashmap<String, Hashmap<String, String>>
    // structure to hold all newMedia renditions List of <MediaRendition>
    map<string, MediaRendition> renditions;
    // structure to hold all rendition variants List of <Variant>

    // read in file line by line starting with #
    // check & ignore blank lines & whitespace
    public: int readFile(string filename) {
        ifstream fin;
        fin.open(filename);
        string nextLine;

        //cout << "Reading from the file" << endl; 
        getline(fin, nextLine);
        if (nextLine == "#EXTM3U") {
            isPlaylist = true;
            //cout << "is playlist" << endl;
        }

        // validate Master plylist rules
        // Global vs Instance Tags ":"
        while(fin.good()) {
            nextLine = "";
            getline(fin, nextLine);
            int pos = 0;
        
            if (!nextLine.empty()) {
                string newMedia = nextLine;
                int index;
                // Use Tag to create & set Rendition or Variant instance
                if ((index = newMedia.find("#EXT-X-MEDIA", 0)) != string::npos)
                {
                    // cout << "newMedia type" << endl;
                    MediaRendition med;
                    med.Parse(newMedia);
                    cout << med.getGroupID() << endl;
                    //renditions[] = med;

                } else if ((index = newMedia.find("#EXT-X-STREAM-INF", 0)) != string::npos)
                {
                    // cout << "var type" << endl;

                } else if (((index = newMedia.find("#EXT-X-I-FRAME-STREAM-INF", 0)) != string::npos))
                {
                    // cout << "frame type" << endl;
                    // cout << nextLine << endl;

                } else {
                    // cout << nextLine << endl;
                }
    
            }
        }

         return 0;
    }

        
        // Parse using specific instance
        // add instance to correct 
    
        //list MediaList(0).get(groupID) | MediaList(0).getAttr.at(ATTR)=VALUE
};

// main() is where program execution begins.
int main() {
    masterPlaylist MP;               // Create instance of masterPlaylist
    MP.readFile("master.m3u8");      // Set input file


   return 0;
}
