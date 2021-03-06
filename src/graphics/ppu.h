/* ppu.h - Declarações PPU */

#ifndef PPU_HEADER
#define PPU_HEADER

#include "../util/types.h"
#include "../execution/state.h"
#include "../mappers/mappers.h"

void ppu(State *st, Mapper *mapper);
void get_sprites(State *st, Mapper *mapper, uint16_t y, uint8_t sprites[8]);
void insertionSort(uint8_t arr[], uint8_t n, uint8_t oam[]);

void graphics_init();
void draw_point(uint16_t x, uint16_t y, uint8_t r, uint8_t g, uint8_t b);
void graphics_update();
void graphics_finish();

#endif
