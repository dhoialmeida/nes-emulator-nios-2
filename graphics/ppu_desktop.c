/* ppu_desktop.c - Implementação das primitivas gráficas para desktop */
#ifndef PLACA
#include <stdio.h>
#include <SDL2/SDL.h>

static SDL_Window *window;
static SDL_Renderer *renderer;

static char title[100] = "";
static char frame_count = 0;
static unsigned int last_time = 0;

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
    last_time = SDL_GetTicks();
}

void draw_point(uint16_t x, uint16_t y, uint8_t r, uint8_t g, uint8_t b) {
    SDL_SetRenderDrawColor(renderer, r, g, b, 0xFF);
    SDL_RenderDrawPoint(renderer, x, y);
}

void graphics_update() {
    frame_count++;
    if (frame_count > 10) {
        sprintf(title, "%.1f fps", 10000.0 / (SDL_GetTicks() - last_time));
        SDL_SetWindowTitle(window, title);
        SDL_RenderPresent(renderer);

        last_time = SDL_GetTicks();
        frame_count = 0;
    }
}

void graphics_finish() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

#endif
