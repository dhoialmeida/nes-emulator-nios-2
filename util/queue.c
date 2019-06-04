/* queue.h - Funções para manipulação da fila de memória */

#ifndef QUEUE_HEADER
#define QUEUE_HEADER

#include "../execution/state.h"
#include "../mappers/mapper_struct.h"

#include "constants.h"
#include "types.h"
#include "queue.h"

#define NIL 0

#define PPU_REG_QUEUE_OFFSET 0
#define VRAM_QUEUE_OFFSET (PPU_REG_QUEUE_OFFSET + 8)
#define OAM_QUEUE_OFFSET (VRAM_QUEUE_OFFSET + 0x800)
#define PALETTE_QUEUE_OFFSET (OAM_QUEUE_OFFSET + 256)
#define FINAL_OFFSET (PALETTE_QUEUE_OFFSET + 0x20)

// Adiciona valor para a fila cujo ultimo elemento está em addr
// Retorna o endereço do novo elemento
uint16_t enqueue(Mapper *mapper, uint16_t addr, uint8_t value) {
    uint16_t top = mapper->st->queue_top++;

    mapper->st->queue[addr].next = top;
    mapper->st->queue[top].cycle = mapper->st->cycles;
    mapper->st->queue[top].data = value;
    mapper->st->queue[top].next = NIL;
    return top;
}

// "Reseta" a fila, descartando elementos antigos
void reset_queue(State *st) {
    int i;
    for (i = PPUCTRL; i <= PPUDATA; i++) {
        st->queue[PPU_REG_QUEUE_OFFSET + i] = st->queue[st->ppu_regs[i]];
        st->queue[PPU_REG_QUEUE_OFFSET + i].cycle = 0;
        st->queue[PPU_REG_QUEUE_OFFSET + i].next = NIL;
        st->ppu_regs[i] = i;
    }

    for (uint16_t i = 0; i < 0x800; i++) {
        st->queue[VRAM_QUEUE_OFFSET + i] = st->queue[st->ppu.vram[i]];
        st->queue[VRAM_QUEUE_OFFSET + i].cycle = 0;
        st->queue[VRAM_QUEUE_OFFSET + i].next = NIL;
        st->ppu.vram[i] = VRAM_QUEUE_OFFSET + i;
    }

    for (uint16_t i = 0; i < 256; i++) {
        st->queue[OAM_QUEUE_OFFSET + i] = st->queue[st->ppu.oam[i]];
        st->queue[OAM_QUEUE_OFFSET + i].cycle = 0;
        st->queue[OAM_QUEUE_OFFSET + i].next = NIL;
        st->ppu.oam[i] = OAM_QUEUE_OFFSET + i;
    }

    for (uint16_t i = 0; i < 0x20; i++) {
        st->queue[PALETTE_QUEUE_OFFSET + i] = st->queue[st->ppu.palette[i]];
        st->queue[PALETTE_QUEUE_OFFSET + i].cycle = 0;
        st->queue[PALETTE_QUEUE_OFFSET + i].next = NIL;
        st->ppu.palette[i] = PALETTE_QUEUE_OFFSET + i;
    }

    st->queue_top = FINAL_OFFSET;
}

#endif
