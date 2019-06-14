/* controller.h - handler do joystick */

#ifndef CONTROLLER_HEADER
#define CONTROLLER_HEADER

#include "../execution/state.h"
#include "../mappers/mapper_struct.h"
#include "../util/types.h"

#define JOYSTICK1 0x4016
#define JOYSTICK2 0x4017

uint8_t get_joystick(uint8_t i, State *st, Mapper *mapper);

#endif
