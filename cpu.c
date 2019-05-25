#include <stdio.h>
#include "headers.h"
#include "operations.h"

short int _(short int addr) { //alterar mapping apenas para escrita
    return addr;
}

void cpu(short int pc_addr, State *st) {
    st->pc = pc_addr;
    short signed int addr, eaddr, result;
    unsigned char opcode;
    unsigned char value1, value2, setStatus;

    while (1) {
        setStatus = 1;
        OP_CODE(opcode); //OPCODES: http://6502.org/tutorials/6502opcodes.html#BRA
        log("Doing: ");
        switch (opcode) {  //MODE: Syntax
            //ADC
            case 0x69: log("ADC #$44\n");
                OP_IMM(value1); ADC(value1);
                break;
            case 0x65: log("ADC $FF\n");
                OP_ZP(value1, 0); ADC(value1);
                break;
            case 0x75: log("ADC $FF, X\n");
                OP_ZP(value1, st->x); ADC(value1);
                break;
            case 0x6D: log("ADC $FFFF\n");
                OP_ABS(value1, 0); ADC(value1);
                break;
            case 0x7D: log("ADC $FFFF, X\n");
                OP_ABS(value1, st->x); ADC(value1);
                break;
            case 0x79: log("ADC $FFFF, Y\n");
                OP_ABS(value1, st->y); ADC(value1);
                break;
            case 0x61: log("ADC ($FF,X)\n");
                OP_INDIR_PRE(value1, st->x); ADC(value1);
                break;
            case 0x71: log("ADC ($FF), Y\n");
                OP_INDIR_POS(value1, st->y); ADC(value1);
                break;

            //AND
            case 0x29: log("AND #$44\n");
                OP_IMM(value1); AND(value1);
                break;
            case 0x25: log("AND $44\n");
                OP_ZP(value1, 0); AND(value1);
                break;
            case 0x35: log("AND $FF, X\n");
                OP_ZP(value1, st->x); AND(value1);
                break;
            case 0x2D: log("AND $FFFF\n");
                OP_ABS(value1, 0); AND(value1);
                break;
            case 0x3D: log("AND $FFFF, X\n");
                OP_ABS(value1, st->x); AND(value1);
                break;
            case 0x39: log("AND $FFFF, Y\n");
                OP_ABS(value1, st->y); AND(value1);
                break;
            case 0x21: log("AND ($ff, X)\n");
                OP_INDIR_PRE(value1, st->x); AND(value1);
                break;
            case 0x31: log("AND ($FF), Y\n");
                OP_INDIR_POS(value1, st->y); AND(value1);
                break;

            //ASL
            case 0x0A: log("ASL A\n");
                ASL(st->a, st->a);
                break;
            case 0x06: log("ASL $44\n");
                OP_ZP(value1, 0); ASL(MEM_AT(eaddr), value1);
                break;
            case 0x16: log("ASL $FF, X\n");
                OP_ZP(value1, st->x); ASL(MEM_AT(eaddr), value1);
                break;
            case 0x0E: log("ASL $FFFF\n");
                OP_ABS(value1, 0); ASL(MEM_AT(eaddr), value1);
                break;
            case 0x1E: log("ASL $FFFF, X\n");
                OP_ABS(value1, st->x); ASL(MEM_AT(eaddr), value1);
                break;

            //BCC
            case 0x90: log("BCC $FF\n");
                if (GET(CARRY) == 0) LEA_REL(st->pc, MEM_AT(st->pc));
                break;

            //BCS
            case 0xB0: log("BCS $FF\n");
                if (GET(CARRY) != 0) LEA_REL(st->pc, MEM_AT(st->pc));
                break;

            //BEQ
            case 0xF0: log("BEQ $FF\n");
                if (GET(ZERO) != 0) LEA_REL(st->pc, MEM_AT(st->pc));
                break;

            //BIT
            case 0x24: log("BIT $FF\n");
                OP_ZP(value1, 0); BIT(value1);
                break;
            case 0x2C:
                OP_ABS(value1, 0); BIT(value1);
                break;

            //BMI
            case 0x30: log("BMI $FF\n");
                if (GET(NEGATIVE) != 0) LEA_REL(st->pc, MEM_AT(st->pc));
                break;

            //BNE
            case 0xD0: log("BNE $FF\n");
                if (GET(ZERO) == 0) LEA_REL(st->pc, MEM_AT(st->pc));
                break;

            //BPL
            case 0x10: log("BPL $FF\n");
                if (GET(NEGATIVE) == 0) LEA_REL(st->pc, MEM_AT(st->pc));
                break;

            //BRK
            case 0x00: log("BRK\n");
                st->pc += 2;
                SET_MEM_AT16(st->sp - 2, st->pc);
                MEM_AT(st->sp - 3) = st->p;
                st->pc = MEM_AT16(0xFFFE);
                SET(BREAK, 1);
                break;

            //BVC
            case 0x50: log("BVC $FF\n");
                if (GET(OVERFLOW) == 0) LEA_REL(st->pc, MEM_AT(st->pc));
                break;

            //BVS
            case 0x70: log("BVS $FF\n");
                if (GET(OVERFLOW) != 0) LEA_REL(st->pc, MEM_AT(st->pc));
                break;

            //CLC
            case 0x18: log("CLC\n");
                SET(CARRY, 0);
                break;

            //CLD
            case 0xD8: log("CLD\n");
                SET(DECIMAL, 0);
                break;

            //CLI
            case 0x58: log("CLI\n");
                SET(INTERRUPT_DISABLE, 0);
                break;

            //CLV
            case 0xB8: log("CLV\n");
                SET(OVERFLOW, 0);
                break;

            //CMP
            case 0xC9: log("CMP #FF\n");
                OP_IMM(value1); CMP(value1);
                break;
            case 0xC5: log("CMP $FF ZP\n");
                OP_ZP(value1, 0); CMP(value1);
                break;
            case 0xD5: log("CMP $FF ZP X\n");
                OP_ZP(value1, st->x); CMP(value1);
                break;
            case 0xCD:  log("CMP $FFFF Abs\n");
                OP_ABS(value1, 0); CMP(value1);
                break;
            case 0xDD: log("CMP $FFFF Abs X\n");
                OP_ABS(value1, st->x); CMP(value1);
                break;
            case 0xD9: log("CMP $FFFF Abs Y\n");
                OP_ABS(value1, st->y); CMP(value1);
                break;
            case 0xC1: log("CMP $FF Indir X\n");
                OP_INDIR_PRE(value1, st->x); CMP(value1);
                break;
            case 0xD1: log("CMP $FF Indir Y\n");
                OP_INDIR_POS(value1, st->y); CMP(value1);
                break;

            //CPX
            case 0xE0: log("CPX #FF\n");
                OP_IMM(value1); CPX(value1);
                break;
            case 0xE4: log("CPX $FF Zp\n");
                OP_ZP(value1, 0); CPX(value1);
                break;
            case 0xEC: log("CPX $FFFF Abs\n");
                OP_ABS(value1, 0); CPX(value1);
                break;

            //CPY
            case 0xC0: log("CPX #FF\n");
                OP_IMM(value1); CPY(value1);
                break;
            case 0xC4: log("CPX $FF Zp\n");
                OP_ZP(value1, 0); CPY(value1);
                break;
            case 0xCC: log("CPX $FFFF Abs\n");
                OP_ABS(value1, 0); CPY(value1);
                break;

            //DEC
            case 0xC6: log("DEC $FF Zp\n");
                OP_ZP(value1, 0); DEC(MEM_AT(eaddr), value1);
                break;
            case 0xD6: log("DEC $FF Zp X\n");
                OP_ZP(value1, st->x); DEC(MEM_AT(eaddr), value1);
                break;
            case 0xCE: log("DEC $FFFF Abs\n");
                OP_ABS(value1, 0); DEC(MEM_AT(eaddr), value1);
                break;
            case 0xDE: log("DEC $FFFF Abs X\n");
                OP_ABS(value1, st->x); DEC(MEM_AT(eaddr), value1);
                break;

            //DEX
            case 0xCA: log("DEX\n");
                DEC(st->x, st->x);
                break;

            //DEY
            case 0x88: log("DEY\n");
                DEC(st->y, st->y);
                break;

            //EOR
            case 0x49: log("EOR #$44\n");
                OP_IMM(value1); EOR(value1);
                break;
            case 0x45: log("EOR $FF\n");
                OP_ZP(value1, 0); EOR(value1);
                break;
            case 0x55: log("EOR $FF, X\n");
                OP_ZP(value1, st->x); EOR(value1);
                break;
            case 0x4D: log("EOR $FFFF\n");
                OP_ABS(value1, 0); EOR(value1);
                break;
            case 0x5D: log("EOR $FFFF, X\n");
                OP_ABS(value1, st->x); EOR(value1);
                break;
            case 0x59: log("EOR $FFFF, Y\n");
                OP_ABS(value1, st->y); EOR(value1);
                break;
            case 0x41: log("EOR ($FF,X)\n");
                OP_INDIR_PRE(value1, st->x); EOR(value1);
                break;
            case 0x51: log("EOR ($FF), Y\n");
                OP_INDIR_POS(value1, st->y); EOR(value1);
                break;

            //INC
            case 0xE6: log("INC $FF Zp\n");
                OP_ZP(value1, 0); INC(MEM_AT(eaddr), value1);
                break;
            case 0xF6: log("INC $FF Zp X\n");
                OP_ZP(value1, st->x); INC(MEM_AT(eaddr), value1);
                break;
            case 0xEE: log("INC $FFFF Abs\n");
                OP_ABS(value1, 0); INC(MEM_AT(eaddr), value1);
                break;
            case 0xFE: log("INC $FFFF Abs X\n");
                OP_ABS(value1, st->x); INC(MEM_AT(eaddr), value1);
                break;

            //INX
            case 0xE8: log("INX\n");
                INC(st->x, st->x);
                break;

            //INY
            case 0xC8: log("INY\n");
                INC(st->y, st->y);
                break;

            //JMP
            case 0x4C: log("JMP $FFFF Abs\n");
                OP_ABS(value1, 0); st->pc = eaddr;
                break;
            case 0x6C: log("JMP $FFFF Indir\n");
                OP_INDIR(value1); st->pc = eaddr;
                break;

            //JSR
            case 0x20: log("JMP $FFFF Abs\n");
                OP_ABS(value1, 0);
                SET_MEM_AT16(st->sp - 2, st->pc + 1);
                st->sp -= 2;
                st->pc = eaddr;
                break;

            //LDA
            case 0xA9: log("LDA #$44\n");
                OP_IMM(value1); LDA(st->a, value1);
                break;
            case 0xA5: log("LDA $FF\n");
                OP_ZP(value1, 0); LDA(st->a, value1);
                break;
            case 0xB5: log("LDA $FF, X\n");
                OP_ZP(value1, st->x); LDA(st->a, value1);
                break;
            case 0xAD: log("LDA $FFFF\n");
                OP_ABS(value1, 0); LDA(st->a, value1);
                break;
            case 0xBD: log("LDA $FFFF, X\n");
                OP_ABS(value1, st->x); LDA(st->a, value1);
                break;
            case 0xB9: log("LDA $FFFF, Y\n");
                OP_ABS(value1, st->y); LDA(st->a, value1);
                break;
            case 0xA1: log("LDA ($FF,X)\n");
                OP_INDIR_PRE(value1, st->x); LDA(st->a, value1);
                break;
            case 0xB1: log("LDA ($FF), Y\n");
                OP_INDIR_POS(value1, st->y); LDA(st->a, value1);
                break;

            //LDX
            case 0xA2: log("LDX #$44\n");
                OP_IMM(value1); LDA(st->x, value1);
                break;
            case 0xA6: log("LDX $FF\n");
                OP_ZP(value1, 0); LDA(st->x, value1);
                break;
            case 0xB6: log("LDX $FF\n");
                OP_ZP(value1, st->y); LDA(st->x, value1);
                break;
            case 0xAE: log("LDX $FFFF\n");
                OP_ABS(value1, 0); LDA(st->x, value1);
                break;
            case 0xBE: log("LDX $FFFF, Y\n");
                OP_ABS(value1, st->y); LDA(st->x, value1);
                break;

            //LDY
            case 0xA0: log("LDY #$44\n");
                OP_IMM(value1); LDA(st->y, value1);
                break;
            case 0xA4: log("LDY $FF\n");
                OP_ZP(value1, 0); LDA(st->y, value1);
                break;
            case 0xB4: log("LDY $FF\n");
                OP_ZP(value1, st->x); LDA(st->y, value1);
                break;
            case 0xAC: log("LDY $FFFF\n");
                OP_ABS(value1, 0); LDA(st->y, value1);
                break;
            case 0xBC: log("LDY $FFFF, Y\n");
                OP_ABS(value1, st->x); LDA(st->y, value1);
                break;

            //LSR
            case 0x4A: log("LSR A\n");
                LSR(st->a, st->a);
                break;
            case 0x46: log("LSR $44\n");
                OP_ZP(value1, 0); LSR(MEM_AT(eaddr), value1);
                break;
            case 0x56: log("LSR $FF, X\n");
                OP_ZP(value1, st->x); LSR(MEM_AT(eaddr), value1);
                break;
            case 0x4E: log("LSR $FFFF\n");
                OP_ABS(value1, 0); LSR(MEM_AT(eaddr), value1);
                break;
            case 0x5E: log("LSR $FFFF, X\n");
                OP_ABS(value1, st->x); LSR(MEM_AT(eaddr), value1);
                break;

            //NOP
            case 0xEA: log("NOP\n");
                break;

            //ORA
            case 0x09: log("ORA #$44\n");
                OP_IMM(value1); ORA(value1);
                break;
            case 0x05: log("ORA $FF\n");
                OP_ZP(value1, 0); ORA(value1);
                break;
            case 0x15: log("ORA $FF, X\n");
                OP_ZP(value1, st->x); ORA(value1);
                break;
            case 0x0D: log("ORA $FFFF\n");
                OP_ABS(value1, 0); ORA(value1);
                break;
            case 0x1D: log("ORA $FFFF, X\n");
                OP_ABS(value1, st->x); ORA(value1);
                break;
            case 0x19: log("ORA $FFFF, Y\n");
                OP_ABS(value1, st->y); ORA(value1);
                break;
            case 0x01: log("ORA ($FF,X)\n");
                OP_INDIR_PRE(value1, st->x); ORA(value1);
                break;
            case 0x11: log("ORA ($FF), Y\n");
                OP_INDIR_POS(value1, st->y); ORA(value1);
                break;

            //PHA
            case 0x48: log("PHA\n");
                MEM_AT(st->sp - 1) = st->a;
                st->sp--;
                break;

            //PHP
            case 0x08: log("PHP\n");
                MEM_AT(st->sp - 1) = st->p;
                st->sp--;
                break;

            //PLA
            case 0x68: log("PLA\n");
                st->a = MEM_AT(st->sp);
                st->sp++;
                break;

            //PLP
            case 0x28: log("PLP\n");
                st->p = MEM_AT(st->sp);
                st->sp++;
                break;

            //ROL
            case 0x2A: log("ROL A\n");
                ROL(st->a, st->a);
                break;
            case 0x26: log("ROL $44\n");
                OP_ZP(value1, 0); ROL(MEM_AT(eaddr), value1);
                break;
            case 0x36: log("ROL $FF, X\n");
                OP_ZP(value1, st->x); ROL(MEM_AT(eaddr), value1);
                break;
            case 0x2E: log("ROL $FFFF\n");
                OP_ABS(value1, 0); ROL(MEM_AT(eaddr), value1);
                break;
            case 0x3E: log("ROL $FFFF, X\n");
                OP_ABS(value1, st->x); ROL(MEM_AT(eaddr), value1);
                break;

            //ROR
            case 0x6A: log("ROR A\n");
                ROR(st->a, st->a);
                break;
            case 0x66: log("ROR $44\n");
                OP_ZP(value1, 0); ROR(MEM_AT(eaddr), value1);
                break;
            case 0x76: log("ROR $FF, X\n");
                OP_ZP(value1, st->x); ROR(MEM_AT(eaddr), value1);
                break;
            case 0x6E: log("ROR $FFFF\n");
                OP_ABS(value1, 0); ROR(MEM_AT(eaddr), value1);
                break;
            case 0x7E: log("ROR $FFFF, X\n");
                OP_ABS(value1, st->x); ROR(MEM_AT(eaddr), value1);
                break;

            //RTI
            case 0x40: log("RTI\n");
                st->p = MEM_AT(st->sp);
                st->pc = MEM_AT16(st->sp + 1);
                st->sp += 3;
                break;

            //RTS
            case 0x60: log("RTS\n");
                st->pc = MEM_AT16(st->sp) + 1;
                st->sp += 2;
                break;

            //SBC
            case 0xE9: log("SBC #$44\n");
                OP_IMM(value1); SBC(value1);
                break;
            case 0xE5: log("SBC $FF\n");
                OP_ZP(value1, 0); SBC(value1);
                break;
            case 0xF5: log("SBC $FF, X\n");
                OP_ZP(value1, st->x); SBC(value1);
                break;
            case 0xED: log("SBC $FFFF\n");
                OP_ABS(value1, 0); SBC(value1);
                break;
            case 0xFD: log("SBC $FFFF, X\n");
                OP_ABS(value1, st->x); SBC(value1);
                break;
            case 0xF9: log("SBC $FFFF, Y\n");
                OP_ABS(value1, st->y); SBC(value1);
                break;
            case 0xE1: log("SBC ($FF,X)\n");
                OP_INDIR_PRE(value1, st->x); SBC(value1);
                break;
            case 0xF1: log("SBC ($FF), Y\n");
                OP_INDIR_POS(value1, st->y); SBC(value1);
                break;

            //SEC
            case 0x38: log("SEC\n");
                SET(CARRY, 1);
                break;

            //SED
            case 0xF8: log("SED\n");
                SET(DECIMAL, 1);
                break;

            //SEI
            case 0x78: log("SEI\n");
                SET(INTERRUPT_DISABLE, 1);
                break;

            //STA
            case 0x85: log("STA $FF\n");
                OP_ZP(value1, 0); STA(eaddr);
                break;
            case 0x95: log("STA $FF, X\n");
                OP_ZP(value1, st->x); STA(eaddr);
                break;
            case 0x8D: log("STA $FFFF\n");
                OP_ABS(value1, 0); STA(eaddr);
                break;
            case 0x9D: log("STA $FFFF, X\n");
                OP_ABS(value1, st->x); STA(eaddr);
                break;
            case 0x99: log("STA $FFFF, Y\n");
                OP_ABS(value1, st->y); STA(eaddr);
                break;
            case 0x81: log("STA ($FF,X)\n");
                OP_INDIR_PRE(value1, st->x); STA(eaddr);
                break;
            case 0x91: log("STA ($FF), Y\n");
                OP_INDIR_POS(value1, st->y); STA(eaddr);
                break;

            //STX
            case 0x86: log("STX $FF\n");
                OP_ZP(value1, 0); STX(eaddr);
                break;
            case 0x96: log("STX $FF, Y\n");
                OP_ZP(value1, st->y); STX(eaddr);
                break;
            case 0x8E: log("STX $FFFF\n");
                OP_ABS(value1, 0); STX(eaddr);
                break;

            //STY
            case 0x84: log("STY $FF\n");
                OP_ZP(value1, 0); STY(eaddr);
                break;
            case 0x94: log("STY $FF, X\n");
                OP_ZP(value1, st->x); STY(eaddr);
                break;
            case 0x8C: log("STY $FFFF\n");
                OP_ABS(value1, 0); STY(eaddr);
                break;

            //TAX
            case 0xAA: log("TAX\n");
                setStatus = 0;
                TRN(st->x, st->a);
                SET(ZERO, !st->x);
                SET(NEGATIVE, st->x < 0);
                break;

            //TAY
            case 0xA8: log("TAY\n");
                setStatus = 0;
                TRN(st->y, st->a);
                SET(ZERO, !st->y);
                SET(NEGATIVE, st->y < 0);
                break;

            //TSX
            case 0xBA: log("TSX\n");
                setStatus = 0;
                TRN(st->x, st->sp);
                SET(ZERO, !st->x);
                SET(NEGATIVE, st->x < 0);
                break;

            //TXA
            case 0x8A: log("TXA\n");
                setStatus = 0;
                TRN(st->a, st->x);
                SET(ZERO, !st->a);
                SET(NEGATIVE, st->a < 0);
                break;

            //TXS
            case 0x9A: log("TXS\n");
                TRN(st->sp, st->x);
                break;

            //TYA
            case 0x98: log("TYA\n");
                setStatus = 0;
                TRN(st->a, st->y);
                SET(ZERO, !st->a);
                SET(NEGATIVE, st->a < 0);
                break;

            default:
                log("Invalid! %02X at @%02X\n", opcode, st->pc);
        }

        if (setStatus) {
            SET(CARRY, result > 255);
            SET(OVERFLOW, result > 127 || result < -128);
            SET(NEGATIVE, result < 0);
            SET(ZERO, result == 0);
        }
    }
}
