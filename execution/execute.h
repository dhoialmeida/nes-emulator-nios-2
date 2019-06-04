/* execute.h - função principal do emulador */

#ifndef EXECUTE_HEADER
#define EXECUTE_HEADER

#include "../util/types.h"

#define CYCLES_PER_FRAME 29780

void execute(uint8_t *cartridge, uint32_t start_address);

#endif
