/* execute.h - função principal do emulador */

#ifndef EXECUTE_HEADER
#define EXECUTE_HEADER

#include "../execution/cpu.h"
#include "../execution/state.h"
#include "../mappers/mapper_struct.h"
#include "../util/types.h"

#define CYCLES_PER_FRAME 29780

void execute(uint8_t *cartridge, uint32_t start_address);
void handle_interrupts(State *st, Mapper *mapper);

#endif
