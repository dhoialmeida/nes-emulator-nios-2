/* cpu.h - Declarações CPU */

#ifndef CPU_HEADER
#define CPU_HEADER

#include "../mappers/mappers.h"

#include "state.h"

#define STACK_PAGE 0x0100

// Obtém o valor de memória salvo no endereço addr (16 bits)
#define MEM_AT16(mapper, addr) ((cpu_get(mapper, (addr)+1) << 8) + cpu_get(mapper, addr))
// Altera o valor na memória salvo no endereço addr (16 bits)
#define SET_MEM_AT16(mapper, addr, value) cpu_set(mapper, addr, (value)); cpu_set(mapper, addr+1, ((value) >> 8))


uint8_t cpu(State *st, Mapper *mapper);

#endif
