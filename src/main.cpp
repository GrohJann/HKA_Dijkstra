/**
 * @file main.cpp
 * @brief TODO: Add a short description of what this file does.
 *
 * @author GrohJann
 * @date 12/11/2025
 */

/*
 * Projektarbeit 1 – Dijkstra‘s Algorithmus
 * Gruppe:
 * Jannis Maximilian Grohmann
 *
 * Ich stimme der Veröffentlichung des Source Code *nicht* zu.
 * Copyright (C) 2025 Jannis Maximilian Grohmann
 * SPDX-License-Identifier: CC BY-NC-ND-4.0 (Creative Commons Attribution-NonCommercial-NoDerivatives)
 * https://creativecommons.org/licenses/by-nc-nd/4.0/deed.en
*/

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
//#include <SDL3/SDL_events.h>

#include "Model/OSMHandler.cpp"
#include "View/SDLHandler.h"
#include "Model/Color.h"
#include "Controller/Dijkstra.cpp"


struct AppState {
    const char* title;
    SDL_Window* window;
    SDL_Renderer* renderer;
    Color background;
    Color foreground;
    Color routeColor;
    SDLHandler* sdl_handler;
    OSMHandler* osm_handler;
    Dijkstra* dijkstra;
    bool redraw;
};


/*
 * Called once before anything else<br>
 * Shouldn't have a infinite loop and do only one-time startup stuff
 * @param appstate
 * @param argc
 * @param argv
 * @return if SDL_APP_CONTINUE is returned, the app runs<br>
 *         if SDL_APP_FALIURE is returned, the app terminates with an
 *         appropriate exit error code<br>
 *         if SDL_APP_SUCCESS is returned, the app terminates in success
 */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {
    //OSMHandler osm;
    auto* osm = new OSMHandler();
    //osm.parseFile(fileName);
    osm -> parseFile("../OSMmaps/maposm.sec");

    auto* dijkstra = new Dijkstra();
    //route von HKA N bau zum Schloss
    std::vector<unsigned long long> path = dijkstra -> calculateFastestPath(osm -> getNodeMap(), 6478027644, 14795723);
    /*std::cout << "Route von HKA zum Schloss: " << std::endl;
    for (auto i : path) {
        std::cout << i << std::endl;
    }//*/
    auto* sdl = new SDLHandler(osm -> getNodeMap());

    if (!(*appstate = SDL_malloc(sizeof(AppState)))) {
        return SDL_APP_FAILURE;
    }
    auto* state = (AppState*)*appstate;

    if(!SDL_Init(SDL_INIT_EVENTS))
        return SDL_APP_FAILURE;
    // create window and throw error
    state -> title = "My SDL Window";
    state -> background = {0, 0, 0, 255};
    state -> foreground = {0, 255, 0, 255};
    state -> routeColor = {255, 0, 255, 255};
    state -> sdl_handler = sdl;
    state -> osm_handler = osm;
    state -> dijkstra = dijkstra;
    int width = 640;
    int height = 480;
    if (!(state->window = SDL_CreateWindow(state -> title, width, height, SDL_WINDOW_RESIZABLE)))
        return SDL_APP_FAILURE;
    // create renderer and throw error
    if (!(state->renderer = SDL_CreateRenderer(state->window, NULL)))
        return SDL_APP_FAILURE;

    return SDL_APP_CONTINUE;
}

/*
 * Called over and over at display refresh rate, should be as fast as possible
 * @param appstate
 * @return same as SDL_AppInit()
 */
SDL_AppResult SDL_AppIterate(void *appstate) {
    auto* state = (AppState*)appstate;
    if (state->redraw) {
        //Dijkstra dijkstra;
        //todo: make the route change able
        //route von HKA N bau zum Schloss
        //auto path = dijkstra.getFastestPath(state -> osm_handler -> getNodeMap(), 6478027644, 14795723);
        /*std::cout << "Route von HKA zum Schloss: " << std::endl;
        for (auto i : test) {
            std::cout << i << std::endl;
        }//*/

        state->sdl_handler->redrawSDLWindow(state -> window, state -> renderer, state -> osm_handler->getNodeMap(), state -> dijkstra->getFastestPath(),
            &state -> background, &state -> foreground, &state -> routeColor);
        state->redraw = false;
    }
    SDL_Delay(1);
    return SDL_APP_CONTINUE;
}

/*
 * Called whenever sdl event arrives
 * @param appstate
 * @param event
 * @return
 */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event* event) {
    // Handle events here
    auto* state = (AppState*)appstate;
    switch (event->type) {
    case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
        state->redraw = true;
        break;
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

/*
 * Called once before terminating the app - if it didn't crash or was forcibly killed
 * @param appstate
 * @param result
 */
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    //Cleanup here
    auto* state = (AppState*)appstate;
    if (result == SDL_APP_FAILURE)
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error",
            SDL_GetError(), appstate ? state->window : NULL);
    SDL_free(appstate);
}

