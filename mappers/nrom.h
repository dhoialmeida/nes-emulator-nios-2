/* nrom.h - Mapper NROM (mapper 0) */

#ifndef MAPPER_NROM
#define MAPPER_NROM

#include "mappers.h"

typedef struct MapperNROM {
    MAPPER_BASE_FIELDS;
    uint8_t *prg_rom;
    uint8_t *chr_rom;
    uint8_t prg_ram[0x2000];
} MapperNROM;

void nrom_init(MapperNROM *mapper, State *st, uint8_t *cartridge);

#endif