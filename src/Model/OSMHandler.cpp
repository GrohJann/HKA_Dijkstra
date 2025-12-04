/**
 * @file OSMHandler.cpp
 * @brief Parses OpenStreetMap data
 *
 * @author GrohJann
 * @date 12/11/2025
 */

#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <sstream>
#include <vector>
#include <cmath>


#include "OSMStructs.h"



class OSMHandler {

    std::unordered_map<unsigned long long, OSMNo#de> nodeMap;

public:
    OSMHandler() = default;

    /**
     * Reads and parses the given file and stores the read map for later use
     * Expects OSM files in XML format
     * @param fileName location and name of file to parse
     * @return returns 0 on success
     */
    int parseFile(const std::string& fileName) {
        //read file
        std::string fileContent;
        if (readFile(fileName, &fileContent)) {
            std::cerr << "Error: Cannot open file " << fileName << std::endl;
            return 1;
        }

        //remove unnecessary start of file
        uint contentBegin = fileContent.find("<node");
        fileContent.erase(0, contentBegin);

        // add nodes
        while (fileContent.find("<node") != std::string::npos) {
            std::string node = getElement("node", &fileContent);
            addNode(node);
            //std::cout << node << std::endl << std::endl;
        }

        while (fileContent.find("<way") != std::string::npos) {
            std::string node = getElement("way", &fileContent);
            //std::cout << node << std::endl << std::endl;
            addWay(node);
        }

        std::cout << "File parsed!" << std::endl;
        return 0;
    }

    std::unordered_map<unsigned long long, OSMNode>* getNodeMap() {
        return &nodeMap;
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

    //FIXME: check if Element exists
    static std::string getElement(std::string type, std::string* content) {
        const uint element_start = content -> find("<"+type);
        uint element_end;
        std::string filter;
        if ( content -> find("\n", element_start) < content -> find("/>", element_start) ) {
            // multi line node
            filter = "</"+type+">\n";
            element_end = content -> find(filter, element_start);
        } else {
            // single line node
            filter = "/>\n";
            element_end = content -> find(filter, element_start);
        }
        std::string elementString = content -> substr(element_start, element_end - element_start);
        //delete read node
        content -> erase(element_start, element_end + filter.size());
        return elementString;
    }

    void addNode(std::string node) {
        std::string id = getKeyValue(node, "id"); //long long
        std::string lat = getKeyValue(node, "lat"); //double
        std::string lon = getKeyValue(node, "lon");

        //std::cout << id << std::endl;

        if ((id + lat + lon).empty()) {
            std::cerr << "Error: Cannot add node with id: " << id << ", lat: "
            << lat << ", lon: " << lon << std::endl;
            return;
        }

        try {
            unsigned long long id_ll = std::stoll(id);
            nodeMap.emplace(id_ll, OSMNode{id_ll,std::stod(lat),std::stod(lon)});
        } catch (std::exception& e) {
            std::cerr << "Error in OSMHandler::addNode" << std::endl << "Invalid argument in: " << e.what() << std::endl;
        }
    }

    //TODO: add transport mode handling
    void addWay(std::string elem) {
        if (findHighwayType(elem).empty()) {
            return;
        }
        elem.erase(0, elem.find("\n"));
        // save way to vector
        std::vector<unsigned long long> way;
        while (elem.find("<nd") != std::string::npos) {
            std::string node = getKeyValue(elem, "ref");
            elem.erase(0, elem.find("/>")+4);
            way.push_back(std::stoll(node));
        }
        //iterate over vector adding edges to the nodes
        for (unsigned long long& partialWay : way) {
            if (partialWay != way.back()) {
                OSMNode* start = &(nodeMap.find(partialWay)->second);
                OSMNode* end = &(nodeMap.find(*(std::next(&partialWay, 1)))->second);

                const unsigned long long length = calcDistance(start, end);
                start->edges.push_back({start, end , length});
                //todo: set reverse edge only if the street is not one way
                end->edges.push_back({end, start , length});
            }
        }
    }

    /**
     * Calculates the distance between two OSMNodes using the haversine formula and the nodes latitude and longitude
     * @param start start Node
     * @param end end Node
     * @return distance in millimeter
     */
    static unsigned long long calcDistance( const OSMNode* start, const OSMNode* end ) {
        constexpr long int earthRadius = 6371e6;
        const double phi1 = start->lat * M_PI / 180;
        const double phi2 = end->lat * M_PI / 180;
        const double delta_phi = (end->lat - start->lat) * M_PI / 180;
        const double delta_lambda = (end->lon - start->lon) * M_PI / 180;

        const double a = std::sin(delta_phi/2) * std::sin(delta_phi/2) +
                    std::cos(phi1) * std::cos(phi2) *
                    std::sin(delta_lambda/2) *std::sin(delta_lambda/2);

        const double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1-a));

        return earthRadius * c;
    }

    static std::string getKeyValue(const std::string& node, std::string key) {
        key = key + "=\"";
        if (size_t start = node.find(key); start != std::string::npos) {
            start += key.length();
            const size_t end = node.find('\"', start);
            std::string id = node.substr(start, end - start);
            return id;
        } else {
            return "";
        }
    }

    static std::string findHighwayType(const std::string& node) {
        const std::string key = "highway\" v=\"";
        if (size_t start = node.find(key); start != std::string::npos) {
            start += key.length();
            const size_t end = node.find('\"', start);
            std::string id = node.substr(start, end - start);
            return id;
        } else {
            return "";
        }
    }

};
