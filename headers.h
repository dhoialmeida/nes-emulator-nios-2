/* headers.h - Declarações principais */

#ifndef MAIN_HEADERS
#define MAIN_HEADERS

#ifdef PLACA
    typedef unsigned char uint8_t;
    typedef unsigned short int uint16_t;
    typedef unsigned int uint32_t;
#else
    #include <stdint.h>
#endif

#ifndef DISABLE_LOG
    #define log(...) printf(__VA_ARGS__)
#else
    #define log(...)
#endif

#define STACK_PAGE 0x100

//PPU REGISTERS:
#define PPUCTRL 0x0
#define PPUMASK 0x1
#define PPUSTATUS 0x2
#define OAMADDR 0x3
#define PPUSCROLL 0x5
#define PPUADDR 0x6
#define PPUDATA 0x7

typedef struct State {
    uint16_t pc;
    uint8_t p, a, x, y, sp;
    uint8_t ppu_regs[8];
    uint8_t io_regs[26];
    uint8_t memory[0x800];
    struct {
        uint8_t vram[0x800];
        uint8_t oam[256];
        uint8_t pallete[0x20];
    } ppu;
} State;

typedef struct PPU {
    uint16_t cycle;
    uint8_t memory[0x4000];
} PPU;

#include "mappers/mappers.h"

void execute(uint8_t *cartridge, uint32_t start_address);
uint8_t cpu(State *st, Mapper *mapper, uint32_t cycles);
void ppu(State *st);

void graphics_init();
void draw_point(uint16_t x, uint16_t y, uint8_t r, uint8_t g, uint8_t b);
void graphics_update();
void graphics_finish();

#endif
