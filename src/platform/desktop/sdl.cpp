#include <SDL.h>
#include <iostream>
#include <cstdint>

#include "platform/platform.h"
#include "fb/fb.hpp"

SDL_Window *win = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture = NULL;

int sdl_init(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "error initializing SDL: " << SDL_GetError() << std::endl;
        return -1;
    } else {
        win = SDL_CreateWindow("PiratePlayer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, FB_WIDTH, FB_HEIGHT, 0);
        renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE);
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING, FB_WIDTH, FB_HEIGHT);
    }

    return 0;
}

void sdl_refresh() {
    SDL_UpdateTexture(texture, NULL, &framebuffer, FB_WIDTH * sizeof(uint16_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_PumpEvents();
}

int sdl_exit(void) {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}

int sdl_poll(void) {
    SDL_Event event;
    SDL_PollEvent(&event);
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_a:
                return LEFT;
            case SDLK_d:
                return RIGHT;
            case SDLK_SPACE:
                return UP;
            case SDLK_s:
                return DOWN;
        }
    }

    return 0;
}