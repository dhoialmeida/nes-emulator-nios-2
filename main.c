#include <stdio.h>
#include "headers.h"
#define MAX_SIZE 100000

size_t read_file(char *name, char *buffer, size_t size) {
    FILE *f = fopen(name, "r");
    if (f != NULL) {
        return fread(buffer, sizeof(char), size, f);
    }
    return 0;
}

int main(int argc, char *argv[]) {
    State st;
    PPU ppu;
    unsigned char jogo[MAX_SIZE];
    if (argc > 1) {
        read_file(argv[1], jogo, MAX_SIZE);
    } else {
        printf("./emulador nome_da_rom");
        return 1;
    }
    cpu(romRead(jogo, &st, &ppu), &st);
}
