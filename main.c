#include "headers.h"

int main(void) {
    State st;
    PPU ppu;
    unsigned char *jogo = 0x123456576;
    cpu(romRead(jogo, &st, &ppu), &st);
}