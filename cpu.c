#include "headers.h"
#include "operations.h"

short int _(short int addr) { //alterar mapping apenas para escrita
    return addr;
}

void cpu(short int pc_addr, State *st) {
    st->pc = 0;
    short int addr;
    unsigned char opcode;
    unsigned char value1, value2;

    while (1) {
        OPCODE(opcode); //OPCODES: http://6502.org/tutorials/6502opcodes.html#BRA
        switch (opcode) {  //MODE: Syntax 
            //ADC
            case 0x69: //Imm: ADC #$44
                OP_IMM(value1); ADC(value1); break;
            case 0x65: //ZP: ADC $FF
                OP_ZP(value1, 0); ADC(value1); break;
            case 0x75: //ZP, X: ADC $FF, X
                OP_ZP(value1, st->x); ADC(value1); break;
            case 0x6D: //Abs: ADC $FF00
                OP_ABS(value1, 0); ADC(value1); break;
            case 0x7D: //Abs, X: ADC $FF00, X
                OP_ABS(value1, st->x); ADC(value1); break;
            case 0x79: //Abs, Y: ADC $FF00, Y
                OP_ABS(value1, st->y); ADC(value1); break;
            case 0x61: //Ind, X: ADC ($FF,X)
                break;
            case 0x71: //Ind, Y: ADC ($FF), Y
                break;

            //AND
            case 0x29: //Imm: AND #$44
                break;
            case 0x25: //ZP: AND $44
                break;
            case 0x35: //ZP, X: AND $FF, X
                break;
            case 0x2D: //ABS: AND $FF00
                break;
            case 0x3D: break;
            case 0x39: break;
            case 0x21: break;
            case 0x31: break;

            //ASL
            case 0x0A: break;
            case 0x06: break;
            case 0x16: break;
            case 0x0E: break;
            case 0x1E: break;

            //BCC
            case 0x90: break;

            //BCS
            case 0xB0: break;

            //BEQ
            case 0xF0: break;

            //BIT
            case 0x24: break;
            case 0x2C: break;

            //BMI
            case 0x30: break;
            
            //BNE
            case 0xD0: break;

            //BPL
            case 0x10: break;

            //BRK
            case 0x00: break;

            //BVC
            case 0x50: break;

            //BVS
            case 0x70: break;

            //CLC
            case 0x18: break;

            //CLD
            case 0xD8: break;

            //CLI
            case 0x58: break;

            //CLV
            case 0xB8: break;

            //CMP
            case 0xC9: break;
            case 0xC5: break;
            case 0xD5: break;
            case 0xCD: break;
            case 0xDD: break;
            case 0xD9: break;
            case 0xC1: break;
            case 0xD1: break;
            
            //CPX
            case 0xE0: break;
            case 0xE4: break;
            case 0xEC: break;

            //CPY
            case 0xC0: break;
            case 0xC4: break;
            case 0xCC: break;

            //DEC
            case 0xC6: break;
            case 0xD6: break;
            case 0xCE: break;
            case 0xDE: break;

            //DEX
            case 0xCA: break;

            //DEY
            case 0x88: break;

            //EOR
            case 0x49: break;
            case 0x45: break;
            case 0x55: break;
            case 0x4D: break;
            case 0x5D: break;
            case 0x59: break;
            case 0x41: break;
            case 0x51: break;

            //INC
            case 0xE6: break;
            case 0xF6: break;
            case 0xEE: break;
            case 0xFF: break;

            //INX
            case 0xE8: break;

            //INY
            case 0xC8: break;

            //JMP
            case 0x4C: break;
            case 0x6C: break;

            //JSR
            case 0x20: break;

            //LDA
            case 0xA9: break;
            case 0xA5: break;
            case 0xB5: break;
            case 0xAD: break;
            case 0xBD: break;
            case 0xB9: break;
            case 0xA1: break;
            case 0xB1: break;

            //LDX
            case 0xA2: break;
            case 0xA6: break;
            case 0xB6: break;
            case 0xAE: break;
            case 0xBE: break;

            //LDY
            case 0xA0: break;
            case 0xA4: break;
            case 0xB4: break;
            case 0xAC: break;
            case 0xBC: break;

            //LSR
            case 0x4A: break;
            case 0x46: break;
            case 0x56: break;
            case 0x4E: break;
            case 0x5E: break;

            //NOP
            case 0xEA: break;

            //ORA
            case 0x09: break;
            case 0x05: break;
            case 0x15: break;
            case 0x0D: break;
            case 0x1D: break;
            case 0x19: break;
            case 0x01: break;
            case 0x11: break;

            //PHA
            case 0x48: break;

            //PHP
            case 0x08: break;

            //PLA
            case 0x68: break;

            //PLP
            case 0x28: break;

            //ROL
            case 0x2A: break;
            case 0x26: break;
            case 0x36: break;
            case 0x2E: break;
            case 0x3E: break;

            //ROR
            case 0x6A: break;
            case 0x66: break;
            case 0x76: break;
            case 0x6E: break;
            case 0x7E: break;

            //RTI
            case 0x40: break;

            //RTS
            case 0x60: break;

            //SBC
            case 0xE9: break;
            case 0xE5: break;
            case 0xF5: break;
            case 0xED: break;
            case 0xFD: break;
            case 0xF9: break;
            case 0xE1: break;
            case 0xF1: break;

            //SEC
            case 0x38: break;

            //SED
            case 0xF8: break;

            //SEI
            case 0x78: break;

            //STA
            case 0x85: break;
            case 0x95: break;
            case 0x8D: break;
            case 0x9D: break;
            case 0x99: break;
            case 0x81: break;
            case 0x91: break;

            //STX
            case 0x86: break;
            case 0x96: break;
            case 0x8E: break;

            //STY
            case 0x84: break;  
            case 0x94: break;  
            case 0x8C: break;

            //TAX
            case 0xAA: break;

            //TAY
            case 0xA8: break;

            //TSX
            case 0xBA: break;

            //TXA
            case 0x8A: break;

            //TXS
            case 0x9A: break;

            //TYA
            case 0x98: break;
        }
    }
}