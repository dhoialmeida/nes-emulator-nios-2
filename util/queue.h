/* queue.h - Funções para manipulação da fila de memória */

#ifndef QUEUE_HEADER
#define QUEUE_HEADER

#include "../execution/state.h"
#include "../mappers/mapper_struct.h"

#include "types.h"

#define NIL 0

#define PPU_REG_QUEUE_OFFSET 0
#define VRAM_QUEUE_OFFSET (PPU_REG_QUEUE_OFFSET + 8)
#define OAM_QUEUE_OFFSET (VRAM_QUEUE_OFFSET + 0x800)
#define PALETTE_QUEUE_OFFSET (OAM_QUEUE_OFFSET + 256)
#define FINAL_OFFSET (PALETTE_QUEUE_OFFSET + 0x20)

uint16_t enqueue(Mapper *mapper, uint16_t addr, uint8_t value);
void reset_queue(State *st);

#endif
