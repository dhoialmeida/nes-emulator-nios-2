#include "headers.h"

short int romRead(unsigned char* cart, State* st, PPU* ppu){
    int prgSize = cart[4]*16000;
    int VSize = cart[5]*8000;
    int idx=0;

    // TODO: MAPPER

    short int prgStart;
    short int prgWriter;

    if(prgSize == 16000){
        prgStart = 0xC000;
    } else {
        prgStart = 0x8000;
    }
    prgWriter = prgStart;

    for(int i = 0; i < prgSize; i++){
        st->memory[prgWriter++] = cart[16 + idx++];
    }
    for(int i = 0; i < VSize; i++){
        ppu->memory[i] = cart[idx+i];
    }
    
    return prgStart;
}