/* execute.c - funcção principal do emulador */

#include "headers.h"
#include "mappers/mappers.h"
#include "mappers/nrom.h"

#define CYCLES_PER_FRAME 29780

// Altera o estado para o estado de power-up
void set_powerup_state(State *st) {
    st->a = 0;
    st->x = 0;
    st->y = 0;
    st->p = 0x24;
    st->sp = 0xFD;

    st->ppu_regs[PPUCTRL] = 0x00;
    st->ppu_regs[PPUMASK] = 0x00;
    st->ppu_regs[PPUSTATUS] = 0xA0;
}

/* Executa o programa contido na rom apontada por cartridge

Caso 0 <= start_address <= 0xFFFF, a execução é iniciada nesse endereço;
Caso contrário, o ponto inicial de execução é dado pelo endereço de reset.
*/
void execute(uint8_t *cartridge, uint32_t start_address) {
    State st;
    MapperNROM mapper;
    uint32_t total_cycles = 7;
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
        cycles = cpu(&st, (Mapper *) &mapper, total_cycles);
        total_cycles += cycles;
        cur += cycles;

        if (cur > CYCLES_PER_FRAME) {
            ppu(&st);
            cur = 0;
        }
    }

    graphics_finish();
}