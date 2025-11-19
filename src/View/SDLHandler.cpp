/**
 * @file SDLHandler.cpp
 * @brief TODO: Add a short description of what this file does.
 *
 * @author GrohJann
 * @date 13/11/2025
 */

#include "SDLHandler.h"

#include <ranges>

// location on HKA campus
//double minLat = 49.0166959, maxLat = 49.0166959, minLon = 8.3903114, maxLon = 8.3903114;



SDLHandler::SDLHandler(std::unordered_map<unsigned long long, OSMNode>* nodeMap) {
    // location on HKA campus
    minLat = 49.0166959, maxLat = 49.0166959, minLon = 8.3903114, maxLon = 8.3903114;
    updateBounds(nodeMap);
}

void SDLHandler::redrawSDLWindow(SDL_Window* window, SDL_Renderer* renderer, const std::unordered_map<unsigned long long, OSMNode>* nodeMap,
        const Color* backgroundColor, const Color* foregroundColor) {

    //background color/
    SDL_SetRenderDrawColor(renderer, backgroundColor -> red,
        backgroundColor -> green,  backgroundColor -> blue, backgroundColor -> alpha);
    SDL_RenderClear(renderer);

    int x, y;
    SDL_GetRenderOutputSize(renderer, &x, &y);
    //text color
    SDL_SetRenderDrawColor(renderer, foregroundColor -> red,
        foregroundColor -> green, foregroundColor -> blue, foregroundColor -> alpha);
    SDL_RenderDebugTextFormat(renderer, 5, 5, "Window size %d x %d", x, y);
    SDL_RenderPresent(renderer);





    int w, h;
    SDL_GetWindowSize(window, &w, &h);

    // Draw nodes
    SDL_SetRenderDrawColor(renderer, foregroundColor -> red,
        foregroundColor -> green, foregroundColor -> blue, foregroundColor -> alpha);
    for (const auto& node : std::views::values(*nodeMap)) {
        int x,y;
        projectLatLon(&node, w, h, &x, &y);
        drawCircle(renderer, x, y, 5);
    }


}

void SDLHandler::updateBounds(std::unordered_map<unsigned long long, OSMNode>* nodeMap) {
    for (const auto& node : std::views::values(*nodeMap)) {
        if (node.lat < minLat) minLat = node.lat;
        if (node.lat > maxLat) maxLat = node.lat;
        if (node.lon < minLon) minLon = node.lon;
        if (node.lon > maxLon) maxLon = node.lon;
    }
}

void SDLHandler::projectLatLon(const OSMNode* node, const int width, const int height, int* x, int* y) {
    *x = static_cast<int>((node->lon - minLon) / (maxLon - minLon) * width);
    *y = static_cast<int>(1.0 - (node->lat - minLat) / (maxLat - minLat)) * height;
}

void SDLHandler::drawCircle(SDL_Renderer *r, const int cx, const int cy, const int radius) {
    for (int w = -radius; w <= radius; w++) {
        for (int h = -radius; h <= radius; h++) {
            if (w*w + h*h <= radius * radius)
                SDL_RenderPoint(r, cx + w, cy + h);
        }
    }
}
