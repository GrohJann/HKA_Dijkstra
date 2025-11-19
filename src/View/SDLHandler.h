/**
 * @file SDLHandler.h
 * @brief TODO: Add a short description of what this file does.
 *
 * @author GrohJann
 * @date 13/11/2025
 */

#ifndef HKA_DIJKSTRA_SDLHANDLER_H
#define HKA_DIJKSTRA_SDLHANDLER_H


#include <SDL3/SDL.h>
#include <unordered_map>

#include "../Model/OSMStructs.h"
#include "../Model/Color.h"

class SDLHandler {

private:
    double minLat{}, maxLat{}, minLon{}, maxLon{};

    static void drawCircle(SDL_Renderer *r, int cx, int cy, int radius);
    void projectLatLon(const OSMNode* node, const int width, const int height, int* x, int* y);

public:
    explicit SDLHandler(std::unordered_map<unsigned long long, OSMNode>* nodeMap);

    void redrawSDLWindow(SDL_Window* window, SDL_Renderer* renderer, const std::unordered_map<unsigned long long, OSMNode>* nodeMap, const Color* backgroundColor, const Color* foregroundColor);
    void updateBounds(std::unordered_map<unsigned long long, OSMNode>* nodeMap);
};


#endif //HKA_DIJKSTRA_SDLHANDLER_H