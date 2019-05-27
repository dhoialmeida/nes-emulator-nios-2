#ifdef PLACA
    typedef unsigned char uint8_t;
    typedef unsigned short int uint16_t;
    typedef unsigned int uint32_t;
#else
    #include <stdint.h>
#endif

#define log(...) printf(__VA_ARGS__)
#define STACK_PAGE 0x100

//PPU REGISTERS:
#define PPUCTRL 0x2000
#define PPUMASK 0x2001
#define PPUSTATUS 0x2002
#define OAMADDR 0x2003
#define PPUSCROLL 0x2005
#define PPUADDR 0x2006
#define PPUDATA 0x2007

typedef struct State {
    uint16_t pc;
    uint8_t p, a, x, y, sp;
    uint8_t memory[65536];
} State;

typedef struct PPU {
    uint16_t cycle;
    uint8_t memory[0x4000];
} PPU;

void cpu(uint16_t pc_addr, State *st);

uint16_t romRead(uint8_t* cart, State* st, PPU* ppu);

void ppu_init(State *st);

void ppu_cycle(uint8_t cpu_cycles);