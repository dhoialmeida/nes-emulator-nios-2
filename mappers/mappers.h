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

inline __attribute__((always_inline)) uint16_t enqueue(Mapper *mapper, uint16_t addr, uint8_t value);

inline __attribute__((always_inline)) uint8_t ppu_get(Mapper *mapper, uint16_t addr) {
    return mapper->ppu_get(mapper, addr);
}

inline __attribute__((always_inline)) void ppu_set(Mapper *mapper, uint16_t addr, uint8_t value) {
    mapper->ppu_set(mapper, addr, value);
}

// Obtém o valor no endereço especificado
inline __attribute__((always_inline)) uint8_t cpu_get(Mapper *mapper, uint16_t addr) {
    // RAM
    if (addr < 0x2000) {
        return mapper->st->memory[addr & 0x07FF];
    }

    // PPU
    if (addr < 0x4000) {
        uint8_t read = mapper->st->ppu.status_read;
        uint32_t cyc = mapper->st->cycles;
        uint8_t v = 0;
        switch (addr & 0x7) {
            case PPUSTATUS:
                mapper->st->ppu.addr = 0;
                if (!read && cyc > 27394 && cyc < 29894) {
                    mapper->st->ppu.status_read = 1;
                    v = 0x80;
                } else {
                    v = 0;
                }
                return v | (mapper->st->ppu.last_reg_written & 0x1F);
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
inline __attribute__((always_inline)) void cpu_set(Mapper *mapper, uint16_t addr, uint8_t value) {
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

#endif
