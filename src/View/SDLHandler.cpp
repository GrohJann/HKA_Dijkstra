/**
 * @file SDLHandler.cpp
 * @brief Handles everything SDL, mainly redrawing the window and a few helper
 *        methods necessary to do so
 *
 * @author GrohJann
 * @date 13/11/2025
 */

#include "SDLHandler.h"

#include <ranges>
#include <cmath>
#include <algorithm>


/**
 * Initializes class and updates the upper and lower bounds of the map to be drawn
 * @param nodeMap node map of all the nodes that are supposed to be drawn
 */
SDLHandler::SDLHandler(std::unordered_map<unsigned long long, OSMNode>* nodeMap) {
    // location on HKA campus
    this -> minLat = 49.0166959, this -> maxLat = 49.0166959, this -> minLon = 8.3903114, this -> maxLon = 8.3903114;
    updateBounds(nodeMap);

}

/**
 * Redraws Window on change
 * @param window
 * @param renderer
 * @param nodeMap unordered map containing all Nodes of the Map
 * @param path list of nodes of the calculated route
 * @param backgroundColor
 * @param foregroundColor
 * @param routeColor
 */
void SDLHandler::redrawSDLWindow(
        SDL_Window* window,
        SDL_Renderer* renderer,
        const std::unordered_map<unsigned long long, OSMNode>* nodeMap,
        const std::vector<unsigned long long>* path,
        const Color* backgroundColor,
        const Color* foregroundColor,
        const Color* routeColor) {

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


    int w, h;
    SDL_GetWindowSize(window, &w, &h);

    // Draw nodes
    SDL_SetRenderDrawColor(renderer, foregroundColor -> red,
        foregroundColor -> green, foregroundColor -> blue, foregroundColor -> alpha);
    for (const auto& node : std::views::values(*nodeMap)) {
        constexpr int r = 1;
        const double x = ((node.lon - this->minLon)/(this->maxLon - this->minLon))*w;
        const double y = h - ((node.lat - this->minLat)/(this->maxLat - this->minLat))*h;
        const SDL_FRect rect1 = SDL_FRect(x-r, y-r, 2*r, 2*r);
        for (const Edge& edge : node.edges) {
            auto it = std::find(path->begin(), path->end(), edge.prev->id);
            // check if edge is part of the calculated route
            if (it != path->end() && std::next(it) != path->end() && *std::next(it) == edge.next->id) {
                SDL_SetRenderDrawColor(renderer, routeColor -> red, routeColor -> green, routeColor -> blue, routeColor -> alpha);
            } else {
                SDL_SetRenderDrawColor(renderer, foregroundColor -> red, foregroundColor -> green, foregroundColor -> blue, foregroundColor -> alpha);
            }
            const double x2 = ((edge.next->lon - this->minLon)/(this->maxLon - this->minLon))*w;
            const double y2 = h - ((edge.next->lat - this->minLat)/(this->maxLat - this->minLat))*h;
            SDL_RenderLine(renderer, x, y, x2, y2);
        }
        SDL_RenderRect(renderer, &rect1);
    }
    SDL_RenderPresent(renderer);
}

/**
 * Updates the map bounds
 * @param nodeMap map of nodes to be mapped in the future
 */
void SDLHandler::updateBounds(std::unordered_map<unsigned long long, OSMNode>* nodeMap) {
    for (const auto& node : std::views::values(*nodeMap)) {
        if (node.lat < minLat) this -> minLat = node.lat;
        if (node.lat > maxLat) this -> maxLat = node.lat;
        if (node.lon < minLon) this -> minLon = node.lon;
        if (node.lon > maxLon) this -> maxLon = node.lon;
    }
}


/**
 * projects a location from latitude and longitude to cartesian coordinates
 * @param node
 * @param x
 * @param y
 */
[[deprecated("not fully implemented, or working")]]
void SDLHandler::projectLatLon(const double* lat, const double* lon, int* x, int* y) {
    constexpr long int earthRadius = 6371e5;
    *x = earthRadius * std::cos(*lat) * std::cos(*lon);
    *y = earthRadius * std::cos(*lat) * std::sin(*lon);
}
