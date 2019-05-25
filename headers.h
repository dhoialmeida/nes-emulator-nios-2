#define log(...) printf(__VA_ARGS__)

typedef struct State {
    unsigned short int pc, sp;
    unsigned char p, a, x, y;
    unsigned char memory[65536];
} State;

typedef struct PPU {
    unsigned char memory[0x4000];
} PPU;

void cpu(unsigned short int pc_addr, State *st);

short int romRead(unsigned char* cart, State* st, PPU* ppu);
