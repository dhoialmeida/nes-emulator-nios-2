#include <stdint.h>

#define log(...) printf(__VA_ARGS__)

typedef struct State {
    uint16_t pc, sp;
    uint8_t p, a, x, y;
    uint8_t memory[65536];
} State;

typedef struct PPU {
    uint8_t memory[0x4000];
} PPU;

void cpu(uint16_t pc_addr, State *st);

uint16_t romRead(uint8_t* cart, State* st, PPU* ppu);
