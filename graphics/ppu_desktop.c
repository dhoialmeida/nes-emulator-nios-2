/* ppu_desktop.c - Implementação das primitivas gráficas para desktop */
#ifndef PLACA
#include <stdio.h>
#include <SDL2/SDL.h>

static SDL_Window *window;
static SDL_Renderer *renderer;

static char title[100] = "";
static int update_counter = 0;

void graphics_init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    }

    window = SDL_CreateWindow("Update 0",  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 256, 240, SDL_WINDOW_SHOWN);

    if (window == NULL) {
        SDL_Quit();
        return;
    }

    // We create a renderer with hardware acceleration, we also present according with the vertical sync refresh.
    renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) ;
}

void draw_point(uint16_t x, uint16_t y, uint8_t r, uint8_t g, uint8_t b) {
    SDL_SetRenderDrawColor(renderer, r, g, b, 0xFF);
    SDL_RenderDrawPoint(renderer, x, y);
}

void graphics_update() {
    update_counter++;
    sprintf(title, "Update %d", update_counter);
    SDL_SetWindowTitle(window, title);
    SDL_RenderPresent(renderer);
}

void graphics_finish() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

#endif
