/* mappers.c - biblioteca base para mappers */

#include "../execution/controller.h"
#include "../execution/state.h"
#include "../util/constants.h"
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
                return mapper->st->ppu_regs[addr & 0x7];
        }
    }

    // APU / IO
    if (addr < 0x4020) {
        switch (addr) {
        case JOYSTICK1:
            return get_joystick(1, mapper->st, mapper);
            break;

        case JOYSTICK2:
            return get_joystick(2, mapper->st, mapper);
            break;

        default:
            return mapper->st->io_regs[addr & 0xFF];
            break;
        }
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
                mapper->st->ppu.addr += mapper->st->ppu.increment;
                break;
            case PPUCTRL:
                mapper->st->ppu.nmi_output = (value >> 7);
                mapper->st->ppu.increment = (value & 4) ? 32 : 1;
                break;
            case OAMADDR:
                mapper->st->ppu.oam_addr = value;
                break;
            case OAMDATA:
                mapper->st->ppu.oam[mapper->st->ppu.oam_addr] = value;
            default:
                mapper->st->ppu_regs[addr & 7] = value;
                break;
        }
        return;
    }

    // APU / IO
    if (addr < 0x4020) {
        switch (addr) {
        case JOYSTICK1:
            // TODO
            return;
        case JOYSTICK2:
            // TODO
            return;
        case OAMDMA:
            for (uint16_t addr = (value << 8); addr < ((value+1) << 8); addr++) {
                mapper->st->ppu.oam[(mapper->st->ppu.oam_addr++) & 0xFF] = cpu_get(mapper, addr);
            }
            mapper->st->cycles += 514;
            return;
        default:
            mapper->st->io_regs[addr & 0xFF] = value;
            return;
        }
    }

    // Cartrige space
    mapper->cpu_set(mapper, addr, value);
}
