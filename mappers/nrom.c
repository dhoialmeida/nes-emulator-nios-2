/* nrom.c - Implementação do mapper NROM (mapper 0) */

#include <stdio.h>
#include "../headers.h"
#include "nrom.h"

uint8_t nrom_cpu_get(Mapper *mapper, uint16_t addr) {
    MapperNROM *map = (MapperNROM *) mapper;

    // 0x6000 ~ 0x7FFF => PRG RAM
    if (addr < 0x8000) {
        return map->prg_ram[addr - 0x6000];
    }

    // 0x8000 ~ 0xBFFF | 0xC000 ~ 0xFFFF = PRG_ROM, mirrored
    return map->prg_rom[addr & 0x3FFF];
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
        if (map->mirroring == VERTICAL) {
            addr &= 0x7FF;
        } else {
            addr &= (addr & 0xBFF) | ((addr & 0x800) >> 1);
        }
        return map->st->ppu.vram[addr];
    }

    return map->st->ppu.pallete[addr & 0x1F];
}

void nrom_ppu_set(Mapper *mapper, uint16_t addr, uint8_t value) {
    MapperNROM *map = (MapperNROM *) mapper;

    // 0x0000 ~ 0x1FFF => CHR-ROM
    if (addr < 0x2000) {
        map->chr_rom[addr] = value;
        return;
    }

    if (addr < 0x3F00) {
        if (map->mirroring == VERTICAL) {
            addr &= 0x7FF;
        } else {
            addr &= (addr & 0xBFF) | ((addr & 0x800) >> 1);
        }
        map->st->ppu.vram[addr] = value;
        return;
    }

    map->st->ppu.pallete[addr & 0x1F] = value;
}

void nrom_init(MapperNROM *mapper, State *st, uint8_t *cartridge) {
    mapper->st = st;
    mapper->cartridge = cartridge;
    mapper->prg_rom = cartridge + 16;
    mapper->chr_rom = mapper->prg_rom + cartridge[4]*0x4000;
    mapper->mirroring = cartridge[6] & 0x1;

    mapper->cpu_get = nrom_cpu_get;
    mapper->cpu_set = nrom_cpu_set;
    mapper->ppu_get = nrom_cpu_get;
    mapper->ppu_set = nrom_ppu_set;
}
