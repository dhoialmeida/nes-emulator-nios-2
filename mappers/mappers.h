/* mappers.h - biblioteca base para mappers */
#include "../execution/state.h"
#include "../util/constants.h"
#include "../util/queue.h"
#include "../util/types.h"

#include "mapper_struct.h"

// Obtém o valor no endereço especificado
uint8_t ppu_get(Mapper *mapper, uint16_t addr);

// Altera o valor no endereço especificado
void ppu_set(Mapper *mapper, uint16_t addr, uint8_t value);

// Obtém o valor no endereço especificado
uint8_t cpu_get(Mapper *mapper, uint16_t addr);

/* Altera o valor no endereço especificado */
void cpu_set(Mapper *mapper, uint16_t addr, uint8_t value);
