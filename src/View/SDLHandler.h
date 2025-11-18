//
// Created by ruvaak on 13/11/2025.
//

#ifndef HKA_DIJKSTRA_SDLHANDLER_H
#define HKA_DIJKSTRA_SDLHANDLER_H


#include <SDL3/SDL.h>

#include "../Model/Color.h"

class SDLHandler {

public:
    SDLHandler() = default;

    static void redrawSDLWindow(SDL_Window* window, SDL_Renderer* renderer, const Color* backgroundColor, const Color* foregroundColor);
};


#endif //HKA_DIJKSTRA_SDLHANDLER_H