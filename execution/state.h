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

    // Queue
    struct {
        uint32_t cycle;
        uint8_t data;
        uint16_t next;
    } queue[14500];
    uint16_t queue_top;

    // Queued memory
    // Cada elemento desses vetores contém o índice para o último
    // valor colocado na fila.
    uint16_t ppu_regs[8];
    struct {
        uint16_t vram[0x800];
        uint16_t oam[256];
        uint16_t palette[0x20];
        uint16_t addr; // internal, not queued
        uint8_t status_read;
        uint8_t last_reg_written;
    } ppu;
} State;
#endif
