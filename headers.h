typedef struct State {
    short int pc, sp;
    char a, x, y, p;
    unsigned char memory[65536];
} State;

void cpu(short int *rom_addr);


