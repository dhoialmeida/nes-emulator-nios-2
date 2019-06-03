/* mappers.h - biblioteca base para mappers */

#ifndef MAPPERS_LIB
#define MAPPERS_LIB

// Atributos base para um Mapper
#define MAPPER_BASE_FIELDS \
    State *st; \
    uint8_t *cartridge; \
    uint8_t (*cpu_get) (struct Mapper *mapper, uint16_t addr); \
    void (*cpu_set) (struct Mapper *mapper, uint16_t addr, uint8_t value); \
    uint8_t (*ppu_get) (struct Mapper *mapper, uint16_t addr); \
    void (*ppu_set) (struct Mapper *mapper, uint16_t addr, uint8_t value)

// Estrutura geral de um Mapper
typedef struct Mapper {
    MAPPER_BASE_FIELDS;
} Mapper;

// Obtém o valor no endereço especificado
inline __attribute__((always_inline)) uint8_t cpu_get(Mapper *mapper, uint16_t addr) {
    // RAM
    if (addr < 0x2000) {
        return mapper->st->memory[addr & 0x07FF];
    }

    // PPU
    if (addr < 0x4000) {
        // (ultimo da fila)
        return mapper->st->queue[mapper->st->ppu_regs[addr & 0x7]].data;
    }

    // APU / IO
    if (addr < 0x4020) {
        return mapper->st->io_regs[addr & 0xFF];
    }

    // Cartrige space
    return mapper->cpu_get(mapper, addr);
}

/* Altera o valor no endereço especificado */
inline __attribute__((always_inline)) void cpu_set(Mapper *mapper, uint16_t addr, uint8_t value) {
    // RAM
    if (addr < 0x2000) {
        mapper->st->memory[addr & 0x07FF] = value;
        return;
    }

    // PPU
    if (addr < 0x4000) {
        // Insere o novo dado na fila
        uint16_t old = mapper->st->ppu_regs[addr & 7];
        uint16_t top = mapper->st->queue_top++;

        mapper->st->queue[old].next = top;
        mapper->st->queue[top].cycle = mapper->st->cycles;
        mapper->st->queue[top].data = value;
        mapper->st->queue[top].next = NIL;

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

#endif
