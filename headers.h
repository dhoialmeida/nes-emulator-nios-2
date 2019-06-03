/* headers.h - Declarações principais */

#ifndef MAIN_HEADERS
#define MAIN_HEADERS

#ifdef PLACA
    typedef unsigned char uint8_t;
    typedef unsigned short int uint16_t;
    typedef unsigned int uint32_t;
#else
    #include <stdint.h>
#endif

#ifndef DISABLE_LOG
    #define log(...) printf(__VA_ARGS__)
#else
    #define log(...)
#endif

#define STACK_PAGE 0x100

//PPU REGISTERS:
#define PPUCTRL 0x0
#define PPUMASK 0x1
#define PPUSTATUS 0x2
#define OAMADDR 0x3
#define PPUSCROLL 0x5
#define PPUADDR 0x6
#define PPUDATA 0x7

#define NIL 0

#define PPU_REG_QUEUE_OFFSET 0
#define VRAM_QUEUE_OFFSET (PPU_REG_QUEUE_OFFSET + 8)
#define OAM_QUEUE_OFFSET (VRAM_QUEUE_OFFSET + 0x800)
#define PALETTE_QUEUE_OFFSET (OAM_QUEUE_OFFSET + 256)
#define FINAL_OFFSET (PALETTE_QUEUE_OFFSET + 0x20)

typedef struct State {
    uint32_t cycles;
    uint16_t pc;
    uint8_t p, a, x, y, sp;
    uint8_t io_regs[26];
    uint8_t memory[0x800];

    // Queue
    struct {
        uint32_t cycle;
        uint8_t data;
        uint16_t next;
    } queue[14500];
    uint16_t queue_top;

    // Queued memory
    // Cada elemento desses vetores contém o índice para o último
    // valor colocado na fila.
    uint16_t ppu_regs[8];
    struct {
        uint16_t vram[0x800];
        uint16_t oam[256];
        uint16_t palette[0x20];
        uint16_t addr; // internal, not queued
        uint8_t status_read;
        uint8_t last_reg_written;
    } ppu;
} State;

#include "mappers/mappers.h"

void execute(uint8_t *cartridge, uint32_t start_address);
uint8_t cpu(State *st, Mapper *mapper);
void ppu(State *st, Mapper *mapper);

void graphics_init();
void draw_point(uint16_t x, uint16_t y, uint8_t r, uint8_t g, uint8_t b);
void graphics_update();
void graphics_finish();

// Adiciona valor para a fila cujo ultimo elemento está em addr
// Retorna o endereço do novo elemento
inline __attribute__((always_inline)) uint16_t enqueue(Mapper *mapper, uint16_t addr, uint8_t value) {
    uint16_t top = mapper->st->queue_top++;

    mapper->st->queue[addr].next = top;
    mapper->st->queue[top].cycle = mapper->st->cycles;
    mapper->st->queue[top].data = value;
    mapper->st->queue[top].next = NIL;
    return top;
}

void reset_queue(State *st);

#endif
