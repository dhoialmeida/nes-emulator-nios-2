/* mappers.c - biblioteca base para mappers */

#include "../execution/state.h"
#include "../util/constants.h"
#include "../util/queue.h"
#include "../util/types.h"

#include "mapper_struct.h"

uint8_t ppu_get(Mapper *mapper, uint16_t addr) {
    return mapper->ppu_get(mapper, addr);
}

void ppu_set(Mapper *mapper, uint16_t addr, uint8_t value) {
    mapper->ppu_set(mapper, addr, value);
}

// Obtém o valor no endereço especificado
uint8_t cpu_get(Mapper *mapper, uint16_t addr) {
    // RAM
    if (addr < 0x2000) {
        return mapper->st->memory[addr & 0x07FF];
    }

    // PPU
    if (addr < 0x4000) {
        uint8_t v = 0;
        switch (addr & 0x7) {
            case PPUSTATUS:
                mapper->st->ppu.addr = 0;
                v = (mapper->st->ppu.nmi_occurred << 7) | (mapper->st->ppu.last_reg_written & 0x1F);
                mapper->st->ppu.nmi_occurred = 0;
                return v;
                break;
            case PPUADDR:
                return mapper->st->ppu.addr;
            case PPUDATA:
                return ppu_get(mapper, mapper->st->ppu.addr);
                break;
            default:
                return mapper->st->queue[mapper->st->ppu_regs[addr & 0x7]].data;
        }
    }

    // APU / IO
    if (addr < 0x4020) {
        return mapper->st->io_regs[addr & 0xFF];
    }

    // Cartrige space
    return mapper->cpu_get(mapper, addr);
}

/* Altera o valor no endereço especificado */
void cpu_set(Mapper *mapper, uint16_t addr, uint8_t value) {
    // RAM
    if (addr < 0x2000) {
        mapper->st->memory[addr & 0x07FF] = value;
        return;
    }

    // PPU
    if (addr < 0x4000) {
        mapper->st->ppu.last_reg_written = value;
        switch (addr & 0x7) {
            case PPUSTATUS:
                break;
            case PPUADDR:
                mapper->st->ppu.addr <<= 8;
                mapper->st->ppu.addr |= value;
                break;
            case PPUDATA:
                ppu_set(mapper, mapper->st->ppu.addr, value);
                mapper->st->ppu.addr++;
                break;
            case PPUCTRL:
                mapper->st->ppu.nmi_output = (value >> 7);
            default:
                mapper->st->ppu_regs[addr & 7] = enqueue(mapper, mapper->st->ppu_regs[addr & 7], value);
        }
        return;
    }

    // APU / IO
    if (addr < 0x4020) {
        mapper->st->io_regs[addr & 0xFF] = value;
        return;
    }

    // Cartrige space
    mapper->cpu_set(mapper, addr, value);
}