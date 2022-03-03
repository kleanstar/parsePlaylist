#include <iostream>
#include <iterator>
#include <map>
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
* read in file 
* parse media instances
* sort by attributes
*/
class masterPlaylist {
    bool isMaster;
    bool isValid;
    bool independentSegs;
    // structure to hold all master attr Hashmap<String, Hashmap<String, String>>
    // structure to hold all media renditions List of <MediaRendition>
    // structure to hold all rendition variants List of <Variant>

    // read in file line by line starting with #
    // check & ignore blank lines & whitespace

    // validate Master plylist rules

    // Global vs Instance Tags ":"

        // Use Tag to create & set Rendition or Variant instance
        // Parse using specific instance
        // add instance to correct 
        
        
        //list MediaList(0).get(groupID) | MediaList(0).getAttr.at(ATTR)=VALUE


}


/*
 * instance of each media manifestrendition EXT-X-MEDIA
*/
class MediaRendition {
    Hashmap attributes = Hashmap<String, String>;
    enum Type {AUDIO, VIDEO, SUBTITLES, CAPTIONS};
    string groupID;
    string name;

    // func: take in line string & parse for attributes
    Hashmap Parse(String newLine) {
        // parse by "," delimiter excluding CODECS ","
        // create key:value pairs for each "=" within

    }


    // getters for required 

}

/*
 * instance of each variant stream 
 * EXT-X-STREAM-INF | EXT-X-I-FRAME-STREAM-INF
*/
class Variant {
    Hashmap attributes = Hashmap<String, String>;
    string URI;
    int Bandwidth; 
    // if enum TYpe Key present must match some GroupID present

     // func: take in line string & parse for attributes
    Hashmap Parse(String newLine) {
        // parse by "," delimiter excluding CODECS ","
        // create key:value pairs for each "=" within

    }


}


// main() is where program execution begins.
int main() {

    // Create instance of masterPlaylist
    // Set input file

   return 0;
}
