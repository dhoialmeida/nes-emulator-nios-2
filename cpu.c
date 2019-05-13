#include "headers.h"
#include "operations.h"

short int _(short int addr) {
    return addr;
}

void cpu(short int pc_addr, State *st) {
    st->pc = 0;
    short int addr;
    unsigned char opcode;
    unsigned char value1, value2;

    while (1) {
        OPCODE(opcode);
        switch (opcode) {
            case 0x69:
                OP_IMM(value1); ADC(value1); break;
            case 0x65:
                OP_ZP(value1, 0); ADC(value1); break;
            case 0x75:
                OP_ZP(value1, st->x); ADC(value1); break;
            case 0x6D:
                OP_ABS(value1, 0); ADC(value1); break;
            case 0x7D:
                OP_ABS(value1, st->x); ADC(value1); break;
            case 0x79:
                OP_ABS(value1, st->y); ADC(value1); break;
        }
    }
}
