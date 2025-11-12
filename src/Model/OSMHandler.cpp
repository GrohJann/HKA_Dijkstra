//
// Created by ruvaak on 12.11.25.
//

#include <iostream>
#include <string>
#import <fstream>
#include <unordered_map>
#include <sstream>

#include "LinkedList.h"


struct OSMNode {
    long long id;
    double lat;
    double lon;
};

enum TransportMode {
    CAR,
    BIKE,
    FOOT
};

struct OSMWay {
    LinkedList <OSMNode> nodes;
    TransportMode mode;
};




class OSMHandler {

    std::unordered_map<long long, OSMNode> nodeMap;
    std::unordered_map<long long, OSMWay> wayMap;

public:
    OSMHandler() = default;

    /**
     * Reads and parses the given file and stores the read map for later use
     * Expects OSM files in XML format
     * @param fileName location and name of file to parse
     * @return returns 0 on success
     */
    int parseFile(const std::string& fileName) {
        std::string fileContent;
        if (readFile(fileName, &fileContent)) {
            std::cerr << "Error: Cannot open file " << fileName << std::endl;
            return 1;
        }
            std::cout << fileContent << std::endl;
        return 0;
    }

    void addNode() {
        long long id = 2262599526599;
        double lat = 0;
        double lon = 1;

        nodeMap.emplace(3, OSMNode{id,lat,lon});
    }

private:
    /**
     * Reads a File into a string
     * @param fileName location and name of file
     * @param fileContent string to save the files contents into
     * @return returns true, if an error occurred
     */
    static bool readFile(const std::string& fileName, std::string* fileContent) {
        std::ifstream file(fileName);
        if (!file.is_open()) {
            return true;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        *fileContent = buffer.str();
        return false;
    }
};