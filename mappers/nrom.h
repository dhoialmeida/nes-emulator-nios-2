/* nrom.h - Mapper NROM (mapper 0) */

#ifndef NROM_HEADER
#define NROM_HEADER

#include "../execution/state.h"
#include "../graphics/ppu.h"
#include "../util/types.h"
#include "mappers.h"

#define HORIZONTAL 0
#define VERTICAL 1

typedef struct MapperNROM {
    MAPPER_BASE_FIELDS;
    uint8_t *prg_rom_C000;
    uint8_t *prg_rom_8000;
    uint8_t *chr_rom;
    uint8_t mirroring;
    uint8_t prg_ram[0x2000];
    uint16_t mask;
} MapperNROM;

void nrom_init(MapperNROM *mapper, State *st, uint8_t *cartridge);

#endif
