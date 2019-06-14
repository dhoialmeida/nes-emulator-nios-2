/* execute.c - função principal do emulador */

#include "../graphics/ppu.h"
#include "../mappers/mappers.h"
#include "../mappers/nrom.h"
#include "../util/types.h"

#include "cpu.h"
#include "execute.h"
#include "state.h"

// Altera o estado para o estado de power-up
void set_powerup_state(State *st) {
    st->a = 0;
    st->x = 0;
    st->y = 0;
    st->p = 0x24;
    st->sp = 0xFD;
    st->cycles = 7;

    st->ppu.addr = 0x2000;
    st->ppu.nmi_occurred = 0;
    st->ppu.last_reg_written = 0;
    st->ppu.already_set = 0;
    st->ppu.increment = 1;
}

/* Executa o programa contido na rom apontada por cartridge

Caso 0 <= start_address <= 0xFFFF, a execução é iniciada nesse endereço;
Caso contrário, o ponto inicial de execução é dado pelo endereço de reset.
*/
void execute(uint8_t *cartridge, uint32_t start_address) {
    State st;
    MapperNROM mapper;
    uint8_t cycles;

    set_powerup_state(&st);
    nrom_init(&mapper, &st, cartridge);
    graphics_init();

    if (start_address < 0x10000) {
        st.pc = start_address;
    } else {
        st.pc = MEM_AT16((Mapper *) &mapper, 0xFFFC);
    }

    while (1) {
        cycles = cpu(&st, (Mapper *) &mapper);
        st.cycles += cycles;

        handle_interrupts(&st, (Mapper *) &mapper);

        if (st.cycles > CYCLES_PER_FRAME) {
            ppu(&st, (Mapper *) &mapper);
            st.ppu.already_set = 0;
            st.cycles = 0;
        }
    }

    graphics_finish();
}

void handle_interrupts(State *st, Mapper *mapper) {
    if (!st->ppu.already_set && st->cycles > 27398) {
        st->ppu.nmi_occurred = 1;
        st->ppu.already_set = 1;

        // Generates NMI interruption if NMI_OUTPUT is set
        if (st->ppu.nmi_output) {
            st->sp--;
            SET_MEM_AT16(mapper, st->sp, st->pc);
            st->sp--;
            cpu_set(mapper, st->sp, st->p);
            st->sp--;
            st->pc = MEM_AT16(mapper, 0xFFFA);
        }
    } else if (st->cycles > 29895) {
        st->ppu.nmi_occurred = 0;
    }
}
