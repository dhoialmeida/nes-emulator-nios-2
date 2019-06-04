/* cpu.h - Declarações CPU */

#ifndef CPU_HEADER
#define CPU_HEADER

#include "../mappers/mappers.h"

#include "state.h"

#define STACK_PAGE 0x0100

uint8_t cpu(State *st, Mapper *mapper);

#endif
