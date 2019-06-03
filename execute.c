/* execute.c - funcção principal do emulador */

#include "headers.h"
#include "mappers/mappers.h"
#include "mappers/nrom.h"

#define CYCLES_PER_FRAME 29780

// Altera o estado para o estado de power-up
void set_powerup_state(State *st) {
    uint16_t i;
    st->a = 0;
    st->x = 0;
    st->y = 0;
    st->p = 0x24;
    st->sp = 0xFD;
    st->cycles = 7;

    for (i = PPUCTRL; i <= PPUDATA; i++) {
        st->ppu_regs[i] = i;
        st->queue[i].cycle = 0;
        st->queue[i].next = NIL;
    }

    for (uint16_t i = 0; i < 0x800; i++) {
        st->ppu.vram[i] = VRAM_QUEUE_OFFSET + i;
    }

    for (uint16_t i = 0; i < 256; i++) {
        st->ppu.oam[i] = OAM_QUEUE_OFFSET + i;
    }

    for (uint16_t i = 0; i < 0x20; i++) {
        st->ppu.palette[i] = PALETTE_QUEUE_OFFSET + i;
    }

    st->queue[PPUCTRL].data = 0x00;
    st->queue[PPUMASK].data = 0x00;
    st->queue[PPUSTATUS].data = 0xA0;
    st->queue_top = FINAL_OFFSET;

    st->ppu.addr = 0x2000;
}

/* Executa o programa contido na rom apontada por cartridge

Caso 0 <= start_address <= 0xFFFF, a execução é iniciada nesse endereço;
Caso contrário, o ponto inicial de execução é dado pelo endereço de reset.
*/
void execute(uint8_t *cartridge, uint32_t start_address) {
    State st;
    MapperNROM mapper;
    uint16_t cycles, cur = 0;

    set_powerup_state(&st);
    nrom_init(&mapper, &st, cartridge);
    graphics_init();

    if (start_address < 0x10000) {
        st.pc = start_address;
    } else {
        st.pc = cpu_get((Mapper *) &mapper, 0xFFFF);
    }

    while (1) {
        cycles = cpu(&st, (Mapper *) &mapper);
        st.cycles += cycles;
        cur += cycles;

        if (cur > CYCLES_PER_FRAME) {
            ppu(&st, &mapper);
            cur = 0;
        }
    }

    graphics_finish();
}
