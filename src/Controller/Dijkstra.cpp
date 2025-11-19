/**
 * @file Dijkstra.cpp
 * @brief TODO: Add a short description of what this file does.
 *
 * @author GrohJann
 * @date 19/11/2025
 */


#include <iostream>
#include <algorithm>
#include <math.h>
#include <set>
#include <unordered_map>
#include <vector>

#include "../Model/OSMStructs.h"


//struct OSMNode;

class Dijkstra {

    struct nodeInfo{
        unsigned long long id;
        bool visited = false;
        unsigned long long distance = std::numeric_limits<long long>::max();
        unsigned long long previous = std::numeric_limits<long long>::max();

        // sort primarily by distance, then by id
        bool operator<(const nodeInfo& other) const {
            if (distance != other.distance)
                return distance < other.distance;
            return id < other.id;
        }

        // find by id only
        bool operator==(const nodeInfo& other) const {
            return id == other.id;
        }
        bool operator==(const unsigned long long searchedNodeID) const {
            return id == searchedNodeID;
        }

    };


public:
    std::vector<unsigned long long> getFastestPath(const std::unordered_map<unsigned long long, OSMNode>* nodeMap,
                const long long startNodeID, const long long endNodeID) {

        //todo: check if start node has edges
        std::vector<nodeInfo> nodeList;
        nodeList.push_back({nodeMap->find(startNodeID)->second.id, true, 0, std::numeric_limits<long long>::max()});
        // add all nodes accessible from the start Node
        for (const Edge& edge : nodeMap->find(startNodeID)->second.edges) {
            nodeList.push_back({edge.next->id, false, edge.length, edge.prev->id});
        }

        std::sort(nodeList.begin(), nodeList.end());
        uint sorted = 1;

        // get unvisited node with smallest distance
        // if selected node has id of endNode stop searching
        // check if all edges of selected node are in nodeList and update distance as well as previous node as necessary
        // set node as visited
        // sort vector and select next node

        // if at end of vector and end node was not reached throw error

        while (nodeList.begin()+sorted < nodeList.end()) {
            nodeInfo* shortestUnvisitedNode = &*(nodeList.begin()+sorted);
            shortestUnvisitedNode->visited = true;
            if (shortestUnvisitedNode->id == endNodeID) {
                //TODO: handle found endNode
                break;
            }

            for (const Edge& edge : nodeMap->find(shortestUnvisitedNode->id)->second.edges) {

                //create new node from nodeMap
                nodeInfo tmp = {edge.next->id, false, edge.length+shortestUnvisitedNode->distance, edge.prev->id};
                //search if edge.next node already exists in nodeList vector
                nodeInfo search = *(std::find(nodeList.begin()+sorted, nodeList.end(), tmp));
                // node does not exist
                if (search == *nodeList.end()) {
                    nodeList.push_back(tmp);
                } else {
                    if (!search.visited) {
                        unsigned long long distanceCurrentPath = edge.length + shortestUnvisitedNode->distance;
                        search.distance = distanceCurrentPath < search.distance ? distanceCurrentPath : search.distance;
                    }
                }
            }
            // sort again
            std::sort(nodeList.begin()+sorted, nodeList.end());
            // increment sorted
            sorted++;
            //select next sortest unvisited node
        }



        // when end node reached back track to start node and add all visited nodes ids to output vector
        std::vector<unsigned long long> path;
        nodeInfo node = *(nodeList.begin()+sorted);
        path.insert(path.begin(), node.id);

        while (node.previous != std::numeric_limits<long long>::max()) {
            path.insert(path.begin(), node.previous);
            node = *(std::find(nodeList.begin(), nodeList.begin()+sorted +1, node.previous));
        }
        const long double length = static_cast<long double>((nodeList.begin() + sorted)->distance)/static_cast<long double>(100);
        std::cout << "Route gefunden. Laenge: " << length << " Meter" << std::endl;
        return path;
    }
};
