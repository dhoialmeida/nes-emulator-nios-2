#include <stdio.h>
#include <stdlib.h>
#include "headers.h"
#define MAX_SIZE 100000

uint8_t rom_data[MAX_SIZE];

size_t read_file(char *name, uint8_t *buffer, size_t size) {
    FILE *f = fopen(name, "r");
    if (f != NULL) {
        return fread(buffer, sizeof(uint8_t), size, f);
    } else {
        printf("File not found.\n");
        abort();
    }
    return 0;
}

int desktop_main(int argc, char *argv[]) {
    State st;
    PPU ppu;
    uint8_t jogo[MAX_SIZE];
    if (argc > 1) {
        read_file(argv[1], jogo, MAX_SIZE);
    } else {
        printf("./emulador nome_da_rom.\n");
        return 1;
    }
    cpu(romRead(jogo, &st, &ppu), &st);

    return 1;
}

void board_main() {
    State st;
    PPU ppu;
    uint8_t *jogo = rom_data;
    printf("%x\n", rom_data);
    cpu(romRead(jogo, &st, &ppu), &st);
}

int main(int argc, char *argv[]) {
    #ifdef PLACA
        board_main();
    #else
        desktop_main(argc, argv);
    #endif
}