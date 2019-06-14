/* mappers_struct.h - struct base para mappers */

#ifndef MAPPER_STRUCT_HEADER
#define MAPPER_STRUCT_HEADER

#include "../util/types.h"

// Atributos base para um Mapper
#define MAPPER_BASE_FIELDS \
    struct State *st; \
    uint8_t *cartridge; \
    uint8_t (*cpu_get) (struct Mapper *mapper, uint16_t addr); \
    void (*cpu_set) (struct Mapper *mapper, uint16_t addr, uint8_t value); \
    uint8_t (*ppu_get) (struct Mapper *mapper, uint16_t addr); \
    void (*ppu_set) (struct Mapper *mapper, uint16_t addr, uint8_t value)

// Estrutura geral de um Mapper
typedef struct Mapper {
    struct State *st;
    uint8_t *cartridge;
    uint8_t (*cpu_get) (struct Mapper *mapper, uint16_t addr);
    void (*cpu_set) (struct Mapper *mapper, uint16_t addr, uint8_t value);
    uint8_t (*ppu_get) (struct Mapper *mapper, uint16_t addr);
    void (*ppu_set) (struct Mapper *mapper, uint16_t addr, uint8_t value);
} Mapper;

#endif
