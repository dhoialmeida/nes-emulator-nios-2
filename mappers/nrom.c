/* nrom.c - Implementação do mapper NROM (mapper 0) */

#include <stdio.h>

#include "../execution/state.h"
#include "../graphics/ppu.h"
#include "../util/log.h"
#include "../util/types.h"

#include "nrom.h"

uint8_t nrom_cpu_get(Mapper *mapper, uint16_t addr) {
    MapperNROM *map = (MapperNROM *) mapper;

    // 0x6000 ~ 0x7FFF => PRG RAM
    if (addr < 0x8000) {
        return map->prg_ram[addr - 0x6000];
    }

    if (addr >= 0xC000) {
        return map->prg_rom_C000[addr & map->mask];
    }

    return map->prg_rom_8000[addr & map->mask];
}

void nrom_cpu_set(Mapper *mapper, uint16_t addr, uint8_t value) {
    MapperNROM *map = (MapperNROM *) mapper;
    if (addr < 0x8000) {
        map->prg_ram[addr - 0x6000] = value;
    }
    log("!!! Tried to write to ROM!!!");
    return;
}

uint8_t nrom_ppu_get(Mapper *mapper, uint16_t addr) {
    MapperNROM *map = (MapperNROM *) mapper;

    // 0x0000 ~ 0x1FFF => CHR-ROM
    if (addr < 0x2000) {
        return map->chr_rom[addr];
    }

    if (addr < 0x3F00) {
        // if (map->mirroring == VERTICAL) {
        //     addr &= 0x7FF;
        // } else {
        //     addr &= (addr & 0xBFF) | ((addr & 0x800) >> 1);
        // }
        return map->st->ppu.vram[addr - 0x2000];
    }

    return map->st->ppu.palette[addr & 0x1F];
}

void nrom_ppu_set(Mapper *mapper, uint16_t addr, uint8_t value) {
    MapperNROM *map = (MapperNROM *) mapper;

    // 0x0000 ~ 0x1FFF => CHR-ROM
    if (addr < 0x2000) {
        log("!!! Tried to write to ROM!!!");
        return;
    }

    if (addr < 0x3F00) {
        // if (map->mirroring == VERTICAL) {
        //     addr &= 0x7FF;
        // } else {
        //     addr &= (addr & 0xBFF) | ((addr & 0x800) >> 1);
        // }

        map->st->ppu.vram[addr - 0x2000] = value;
        return;
    }

    map->st->ppu.palette[addr & 0x1F] = value;
}

void nrom_init(MapperNROM *mapper, State *st, uint8_t *cartridge) {
    mapper->st = st;
    mapper->cartridge = cartridge;
    mapper->prg_rom_C000 = cartridge + 16;
    mapper->prg_rom_8000 = cartridge + 16 + 16384;
    mapper->chr_rom = mapper->prg_rom_8000 + 32768;
    mapper->mirroring = cartridge[6] & 0x1;
    mapper->mask = 0x7FFF;

    mapper->cpu_get = nrom_cpu_get;
    mapper->cpu_set = nrom_cpu_set;
    mapper->ppu_get = nrom_ppu_get;
    mapper->ppu_set = nrom_ppu_set;
}
