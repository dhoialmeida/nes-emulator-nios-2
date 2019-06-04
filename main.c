/* main.c - programa principal */

#include <stdio.h>
#include <stdlib.h>

#include "execution/execute.h"
#include "util/types.h"

#define MAX_SIZE 100000

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
    uint8_t jogo[MAX_SIZE];

    if (argc > 1) {
        read_file(argv[1], jogo, MAX_SIZE);
    } else {
        printf("./emulador nome_da_rom.\n");
        return 1;
    }

    execute(jogo, 0x10000);
    return 1;
}

void board_main() {
    uint8_t jogo[MAX_SIZE];
    printf("%p\n", jogo);
    execute(jogo, 0xC000);
}

int main(int argc, char *argv[]) {
    #ifdef PLACA
        board_main();
    #else
        desktop_main(argc, argv);
    #endif
}
