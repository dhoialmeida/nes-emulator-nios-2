#include "headers.h"

void ppu_init(State *st){
    st->memory[PPUCTRL] = 0x00;
    st->memory[PPUMASK] = 0x00;
    st->memory[PPUSTATUS] = 0x10100000;
}

void ppu_cycle(int cpu_cycles){

}