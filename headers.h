#define log(x) printf(x "\n")

typedef struct State {
    short int pc, sp;
    char a, x, y, p;
    unsigned char memory[65536];
} State;

typedef struct PPU {
    unsigned char memory[0x4000];
} PPU;


void cpu(short int pc_addr, State *st);
