/* state.h - Estrutura de estado */

#ifndef STATE_HEADER
#define STATE_HEADER

#include "../util/types.h"

typedef struct State {
    uint32_t cycles;
    uint16_t pc;
    uint8_t p, a, x, y, sp;
    uint8_t io_regs[26];
    uint8_t memory[0x800];

    uint8_t ppu_regs[8];
    struct {
        uint8_t vram[0x800];
        uint8_t oam[256];
        uint8_t palette[0x20];
        uint16_t addr;
        uint8_t already_set;
        uint8_t nmi_occurred;
        uint8_t nmi_output;
        uint8_t last_reg_written;
        uint8_t increment;
        uint8_t oam_addr;
    } ppu;
} State;
#endif
