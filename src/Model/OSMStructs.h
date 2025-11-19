/**
 * @file OSMStructs.h
 * @brief TODO: Add a short description of what this file does.
 *
 * @author GrohJann
 * @date 19/11/2025
 */

#ifndef HKA_DIJKSTRA_OSMSTRUCTS_H
#define HKA_DIJKSTRA_OSMSTRUCTS_H

#include <vector>

struct Edge;

struct OSMNode {
    unsigned long long id;
    double lat;
    double lon;
    std::vector<Edge> edges;
};

/*enum TransportMode {
    CAR,
    BIKE,
    FOOT,
    NONE
};*/

struct Edge {
    OSMNode* prev;
    OSMNode* next;
    //TransportMode mode;
    unsigned long long length;
};


#endif //HKA_DIJKSTRA_OSMSTRUCTS_H