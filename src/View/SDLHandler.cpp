//
// Created by ruvaak on 13/11/2025.
//

#include "SDLHandler.h"



void SDLHandler::redrawSDLWindow(SDL_Window* window, SDL_Renderer* renderer,
        const Color* backgroundColor, const Color* foregroundColor) {

    //background color
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
}