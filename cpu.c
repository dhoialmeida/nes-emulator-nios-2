#include "headers.h"
#include "operations.h"

short int _(short int addr) { //alterar mapping apenas para escrita
    return addr;
}

void cpu(short int pc_addr, State *st) {
    st->pc = 0;
    short signed int addr, eaddr, result;
    unsigned char opcode;
    unsigned char value1, value2, setCarryOverflow;

    while (1) {
        setCarryOverflow = 1;
        OP_CODE(opcode); //OPCODES: http://6502.org/tutorials/6502opcodes.html#BRA
        switch (opcode) {  //MODE: Syntax
            //ADC
            case 0x69: log("ADC #$44");
                OP_IMM(value1); ADC(value1);
                break;
            case 0x65: log("ADC $FF");
                OP_ZP(value1, 0); ADC(value1);
                break;
            case 0x75: log("ADC $FF, X");
                OP_ZP(value1, st->x); ADC(value1);
                break;
            case 0x6D: log("ADC $FFFF");
                OP_ABS(value1, 0); ADC(value1);
                break;
            case 0x7D: log("ADC $FFFF, X");
                OP_ABS(value1, st->x); ADC(value1);
                break;
            case 0x79: log("ADC $FFFF, Y");
                OP_ABS(value1, st->y); ADC(value1);
                break;
            case 0x61: log("ADC ($FF,X)");
                OP_INDIR_PRE(value1, st->x); ADC(value1);
                break;
            case 0x71: log("ADC ($FF), Y");
                OP_INDIR_POS(value1, st->y); ADC(value1);
                break;

            //AND
            case 0x29: log("AND #$44");
                OP_IMM(value1); AND(value1);
                break;
            case 0x25: log("AND $44");
                OP_ZP(value1, 0); AND(value1);
                break;
            case 0x35: log("AND $FF, X");
                OP_ZP(value1, st->x); AND(value1);
                break;
            case 0x2D: log("AND $FFFF");
                OP_ABS(value1, 0); AND(value1);
                break;
            case 0x3D: log("AND $FFFF, X");
                OP_ABS(value1, st->x); AND(value1);
                break;
            case 0x39: log("AND $FFFF, Y");
                OP_ABS(value1, st->y); AND(value1);
                break;
            case 0x21: log("AND ($ff, X)");
                OP_INDIR_PRE(value1, st->x); AND(value1);
                break;
            case 0x31: log("AND ($FF), Y");
                OP_INDIR_POS(value1, st->y); AND(value1);
                break;

            //ASL
            case 0x0A: log("ASL A");
                ASL(st->a, st->a);
                break;
            case 0x06: log("ASL $44");
                OP_ZP(value1, 0); ASL(MEM_AT(eaddr), value1);
                break;
            case 0x16: log("ASL $FF, X");
                OP_ZP(value1, st->x); ASL(MEM_AT(eaddr), value1);
                break;
            case 0x0E: log("ASL $FFFF");
                OP_ABS(value1, 0); ASL(MEM_AT(eaddr), value1);
                break;
            case 0x1E: log("ASL $FFFF, X");
                OP_ABS(value1, st->x); ASL(MEM_AT(eaddr), value1);
                break;

            //BCC
            case 0x90: log("BCC $FF");
                if (GET(CARRY) == 0) LEA_REL(st->pc, MEM_AT(st->pc));
                break;

            //BCS
            case 0xB0: log("BCS $FF");
                if (GET(CARRY) != 0) LEA_REL(st->pc, MEM_AT(st->pc));
                break;

            //BEQ
            case 0xF0: log("BEQ $FF");
                if (GET(ZERO) != 0) LEA_REL(st->pc, MEM_AT(st->pc));
                break;

            //BIT
            case 0x24: break;
            case 0x2C: break;

            //BMI
            case 0x30: log("BMI $FF");
                if (GET(NEGATIVE) != 0) LEA_REL(st->pc, MEM_AT(st->pc));
                break;

            //BNE
            case 0xD0: log("BNE $FF");
                if (GET(ZERO) == 0) LEA_REL(st->pc, MEM_AT(st->pc));
                break;

            //BPL
            case 0x10: log("BPL $FF");
                if (GET(NEGATIVE) == 0) LEA_REL(st->pc, MEM_AT(st->pc));
                break;

            //BRK
            case 0x00: break;

            //BVC
            case 0x50: log("BVC $FF");
                if (GET(OVERFLOW) == 0) LEA_REL(st->pc, MEM_AT(st->pc));
                break;

            //BVS
            case 0x70: log("BVS $FF");
                if (GET(OVERFLOW) != 0) LEA_REL(st->pc, MEM_AT(st->pc));
                break;

            //CLC
            case 0x18: log("CLC");
                SET(CARRY, 0);
                break;

            //CLD
            case 0xD8: log("CLD");
                SET(DECIMAL, 0);
                break;

            //CLI
            case 0x58: log("CLI");
                SET(INTERRUPT_DISABLE, 0);
                break;

            //CLV
            case 0xB8: log("CLV");
                SET(OVERFLOW, 0);
                break;

            //CMP
            case 0xC9: log("CMP #FF");
                OP_IMM(value1); CMP(value1);
                break;
            case 0xC5: log("CMP $FF ZP");
                OP_ZP(value1, 0); CMP(value1);
                break;
            case 0xD5: log("CMP $FF ZP X");
                OP_ZP(value1, st->x); CMP(value1);
                break;
            case 0xCD:  log("CMP $FFFF Abs");
                OP_ABS(value1, 0); CMP(value1);
                break;
            case 0xDD: log("CMP $FFFF Abs X");
                OP_ABS(value1, st->x); CMP(value1);
                break;
            case 0xD9: log("CMP $FFFF Abs Y");
                OP_ABS(value1, st->y); CMP(value1);
                break;
            case 0xC1: log("CMP $FF Indir X");
                OP_INDIR_PRE(value1, st->x); CMP(value1);
                break;
            case 0xD1: log("CMP $FF Indir Y");
                OP_INDIR_POS(value1, st->y); CMP(value1);
                break;

            //CPX
            case 0xE0: log("CPX #FF");
                OP_IMM(value1); CPX(value1);
                break;
            case 0xE4: log("CPX $FF Zp");
                OP_ZP(value1, 0); CPX(value1);
                break;
            case 0xEC: log("CPX $FFFF Abs");
                OP_ABS(value1, 0); CPX(value1);
                break;

            //CPY
            case 0xC0: log("CPX #FF");
                OP_IMM(value1); CPY(value1);
                break;
            case 0xC4: log("CPX $FF Zp");
                OP_ZP(value1, 0); CPY(value1);
                break;
            case 0xCC: log("CPX $FFFF Abs");
                OP_ABS(value1, 0); CPY(value1);
                break;

            //DEC
            case 0xC6: log("DEC $FF Zp");
                OP_ZP(value1, 0); DEC(MEM_AT(eaddr), value1);
                break;
            case 0xD6: log("DEC $FF Zp X");
                OP_ZP(value1, st->x); DEC(MEM_AT(eaddr), value1);
                break;
            case 0xCE: log("DEC $FFFF Abs");
                OP_ABS(value1, 0); DEC(MEM_AT(eaddr), value1);
                break;
            case 0xDE: log("DEC $FFFF Abs X");
                OP_ABS(value1, st->x); DEC(MEM_AT(eaddr), value1);
                break;

            //DEX
            case 0xCA:
                DEC(st->x, st->x);
                break;

            //DEY
            case 0x88:
                DEC(st->y, st->y);
                break;

            //EOR
            case 0x49: log("EOR #$44");
                OP_IMM(value1); EOR(value1);
                break;
            case 0x45: log("EOR $FF");
                OP_ZP(value1, 0); EOR(value1);
                break;
            case 0x55: log("EOR $FF, X");
                OP_ZP(value1, st->x); EOR(value1);
                break;
            case 0x4D: log("EOR $FFFF");
                OP_ABS(value1, 0); EOR(value1);
                break;
            case 0x5D: log("EOR $FFFF, X");
                OP_ABS(value1, st->x); EOR(value1);
                break;
            case 0x59: log("EOR $FFFF, Y");
                OP_ABS(value1, st->y); EOR(value1);
                break;
            case 0x41: log("EOR ($FF,X)");
                OP_INDIR_PRE(value1, st->x); EOR(value1);
                break;
            case 0x51: log("EOR ($FF), Y");
                OP_INDIR_POS(value1, st->y); EOR(value1);
                break;

            //INC
            case 0xE6: log("INC $FF Zp");
                OP_ZP(value1, 0); INC(MEM_AT(eaddr), value1);
                break;
            case 0xF6: log("INC $FF Zp X");
                OP_ZP(value1, st->x); INC(MEM_AT(eaddr), value1);
                break;
            case 0xEE: log("INC $FFFF Abs");
                OP_ABS(value1, 0); INC(MEM_AT(eaddr), value1);
                break;
            case 0xFE: log("INC $FFFF Abs X");
                OP_ABS(value1, st->x); INC(MEM_AT(eaddr), value1);
                break;

            //INX
            case 0xE8: log("INX");
                INC(st->x, st->x);
                break;

            //INY
            case 0xC8: log("INY");
                INC(st->y, st->y);
                break;

            //JMP
            case 0x4C: log("JMP $FFFF Abs");
                OP_ABS(value1, 0); st->pc = eaddr;
                break;
            case 0x6C: log("JMP $FFFF Indir");
                OP_INDIR(value1); st->pc = eaddr;
                break;

            //JSR
            case 0x20: log("JMP $FFFF Abs");
                OP_ABS(value1, 0);
                SET_MEM_AT16(st->sp - 2, st->pc + 1);
                st->sp -= 2;
                st->pc = eaddr;
                break;

            //LDA
            case 0xA9: log("LDA #$44");
                OP_IMM(value1); LDA(st->a, value1);
                break;
            case 0xA5: log("LDA $FF");
                OP_ZP(value1, 0); LDA(st->a, value1);
                break;
            case 0xB5: log("LDA $FF, X");
                OP_ZP(value1, st->x); LDA(st->a, value1);
                break;
            case 0xAD: log("LDA $FFFF");
                OP_ABS(value1, 0); LDA(st->a, value1);
                break;
            case 0xBD: log("LDA $FFFF, X");
                OP_ABS(value1, st->x); LDA(st->a, value1);
                break;
            case 0xB9: log("LDA $FFFF, Y");
                OP_ABS(value1, st->y); LDA(st->a, value1);
                break;
            case 0xA1: log("LDA ($FF,X)");
                OP_INDIR_PRE(value1, st->x); LDA(st->a, value1);
                break;
            case 0xB1: log("LDA ($FF), Y");
                OP_INDIR_POS(value1, st->y); LDA(st->a, value1);
                break;

            //LDX
            case 0xA2: log("LDX #$44");
                OP_IMM(value1); LDA(st->x, value1);
                break;
            case 0xA6: log("LDX $FF");
                OP_ZP(value1, 0); LDA(st->x, value1);
                break;
            case 0xB6: log("LDX $FF");
                OP_ZP(value1, st->y); LDA(st->x, value1);
                break;
            case 0xAE: log("LDX $FFFF");
                OP_ABS(value1, 0); LDA(st->x, value1);
                break;
            case 0xBE: log("LDX $FFFF, Y");
                OP_ABS(value1, st->y); LDA(st->x, value1);
                break;

            //LDY
            case 0xA0: log("LDY #$44");
                OP_IMM(value1); LDA(st->y, value1);
                break;
            case 0xA4: log("LDY $FF");
                OP_ZP(value1, 0); LDA(st->y, value1);
                break;
            case 0xB4: log("LDY $FF");
                OP_ZP(value1, st->x); LDA(st->y, value1);
                break;
            case 0xAC: log("LDY $FFFF");
                OP_ABS(value1, 0); LDA(st->y, value1);
                break;
            case 0xBC: log("LDY $FFFF, Y");
                OP_ABS(value1, st->x); LDA(st->y, value1);
                break;

            //LSR
            case 0x4A: log("LSR A");
                LSR(st->a, st->a);
                break;
            case 0x46: log("LSR $44");
                OP_ZP(value1, 0); LSR(MEM_AT(eaddr), value1);
                break;
            case 0x56: log("LSR $FF, X");
                OP_ZP(value1, st->x); LSR(MEM_AT(eaddr), value1);
                break;
            case 0x4E: log("LSR $FFFF");
                OP_ABS(value1, 0); LSR(MEM_AT(eaddr), value1);
                break;
            case 0x5E: log("LSR $FFFF, X");
                OP_ABS(value1, st->x); LSR(MEM_AT(eaddr), value1);
                break;

            //NOP
            case 0xEA: log("NOP");
                break;

            //ORA
            case 0x09: log("ORA #$44");
                OP_IMM(value1); ORA(value1);
                break;
            case 0x05: log("ORA $FF");
                OP_ZP(value1, 0); ORA(value1);
                break;
            case 0x15: log("ORA $FF, X");
                OP_ZP(value1, st->x); ORA(value1);
                break;
            case 0x0D: log("ORA $FFFF");
                OP_ABS(value1, 0); ORA(value1);
                break;
            case 0x1D: log("ORA $FFFF, X");
                OP_ABS(value1, st->x); ORA(value1);
                break;
            case 0x19: log("ORA $FFFF, Y");
                OP_ABS(value1, st->y); ORA(value1);
                break;
            case 0x01: log("ORA ($FF,X)");
                OP_INDIR_PRE(value1, st->x); ORA(value1);
                break;
            case 0x11: log("ORA ($FF), Y");
                OP_INDIR_POS(value1, st->y); ORA(value1);
                break;

            //PHA
            case 0x48: log("PHA");
                MEM_AT(st->sp - 1) = st->a;
                st->sp--;
                break;

            //PHP
            case 0x08: log("PHP");
                MEM_AT(st->sp - 1) = st->p;
                st->sp--;
                break;

            //PLA
            case 0x68: log("PLA");
                st->a = MEM_AT(st->sp);
                st->sp++;
                break;

            //PLP
            case 0x28: log("PLP");
                st->p = MEM_AT(st->sp);
                st->sp++;
                break;

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
            case 0xAA: log("TAX");
                TRN(st->x, st->a);
                SET(ZERO, !st->x);
                SET(NEGATIVE, st->x < 0);
                break;

            //TAY
            case 0xA8: log("TAY");
                TRN(st->y, st->a);
                SET(ZERO, !st->y);
                SET(NEGATIVE, st->y < 0);
                break;

            //TSX
            case 0xBA: log("TSX");
                TRN(st->x, st->sp);
                SET(ZERO, !st->x);
                SET(NEGATIVE, st->x < 0);
                break;

            //TXA
            case 0x8A: log("TXA");
                TRN(st->a, st->x);
                SET(ZERO, !st->a);
                SET(NEGATIVE, st->a < 0);
                break;

            //TXS
            case 0x9A: log("TXS");
                TRN(st->sp, st->x);
                break;

            //TYA
            case 0x98: log("TYA");
                TRN(st->a, st->y);
                SET(ZERO, !st->a);
                SET(NEGATIVE, st->a < 0);
                break;
        }

        if (setCarryOverflow) {
            SET(CARRY, result > 255);
            SET(OVERFLOW, result > 127 || result < -128);
        }
    }
}
