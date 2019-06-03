/* cpu.c - Implementação da cpu */

#include <stdio.h>

#include "headers.h"
#include "mappers/mappers.h"
#include "operations.inc"

uint8_t cpu(State *st, Mapper *mapper) {
    uint16_t result;
    uint16_t addr, eaddr;
    uint8_t opcode, value1, setCV = 0, setZN = 0, cyc = 0;

    log("%04X | A:%02X X:%02X Y:%02X P:%02X SP:%02X CYC:%-6d | ", st->pc, st->a, st->x, st->y, st->p, st->sp & 0xFF, st->cycles);
    OP_CODE(opcode); // OPCODES: http://6502.org/tutorials/6502opcodes.html#BRA
    switch (opcode) {  // MODE: Syntax
        //ADC
        case 0x69: log("ADC ");
            OP_IMM(value1); ADC(value1); cyc += 2;
            break;
        case 0x65: log("ADC ");
            OP_ZP(value1, 0); ADC(value1); cyc += 3;
            break;
        case 0x75: log("ADC ");
            OP_ZP(value1, st->x); ADC(value1); cyc += 4;
            break;
        case 0x6D: log("ADC ");
            OP_ABS(value1, 0, 0); ADC(value1); cyc += 4;
            break;
        case 0x7D: log("ADC ");
            OP_ABS(value1, st->x, 1); ADC(value1); cyc += 4;
            break;
        case 0x79: log("ADC ");
            OP_ABS(value1, st->y, 1); ADC(value1); cyc += 4;
            break;
        case 0x61: log("ADC ");
            OP_INDIR_PRE(value1, st->x); ADC(value1); cyc += 6;
            break;
        case 0x71: log("ADC ");
            OP_INDIR_POS(value1, st->y, 1); ADC(value1); cyc += 5;
            break;

        //AND
        case 0x29: log("AND ");
            OP_IMM(value1); AND(value1); cyc += 2;
            break;
        case 0x25: log("AND ");
            OP_ZP(value1, 0); AND(value1); cyc += 3;
            break;
        case 0x35: log("AND ");
            OP_ZP(value1, st->x); AND(value1); cyc += 4;
            break;
        case 0x2D: log("AND ");
            OP_ABS(value1, 0, 0); AND(value1); cyc += 4;
            break;
        case 0x3D: log("AND ");
            OP_ABS(value1, st->x, 1); AND(value1); cyc += 4;
            break;
        case 0x39: log("AND ");
            OP_ABS(value1, st->y, 1); AND(value1); cyc += 4;
            break;
        case 0x21: log("AND ");
            OP_INDIR_PRE(value1, st->x); AND(value1); cyc += 6;
            break;
        case 0x31: log("AND ");
            OP_INDIR_POS(value1, st->y, 1); AND(value1); cyc += 5;
            break;

        //ASL
        case 0x0A: log("ASL A"); cyc += 2;
            REG_ASL(st->a, st->a);
            break;
        case 0x06: log("ASL ");
            OP_ZP(value1, 0); ASL(eaddr, value1); cyc += 5;
            break;
        case 0x16: log("ASL ");
            OP_ZP(value1, st->x); ASL(eaddr, value1); cyc += 6;
            break;
        case 0x0E: log("ASL ");
            OP_ABS(value1, 0, 0); ASL(eaddr, value1); cyc += 6;
            break;
        case 0x1E: log("ASL ");
            OP_ABS(value1, st->x, 0); ASL(eaddr, value1); cyc += 7;
            break;

        //BCC
        case 0x90: log("BCC ");
            BRANCH(! GET(CARRY));
            break;

        //BCS
        case 0xB0: log("BCS ");
            BRANCH(GET(CARRY));
            break;

        //BEQ
        case 0xF0: log("BEQ ");
            BRANCH(GET(ZERO));
            break;

        //BIT
        case 0x24: log("BIT ");
            OP_ZP(value1, 0); BIT(value1); cyc += 3;
            break;
        case 0x2C: log("BIT ");
            OP_ABS(value1, 0, 0); BIT(value1); cyc += 4;
            break;

        //BMI
        case 0x30: log("BMI ");
            BRANCH(GET(NEGATIVE));
            break;

        //BNE
        case 0xD0: log("BNE ");
            BRANCH(! GET(ZERO));
            break;

        //BPL
        case 0x10: log("BPL ");
            BRANCH(! GET(NEGATIVE));
            break;

        //BRK
        case 0x00: log("BRK");
            st->pc += 2;
            SET_MEM_AT16(STACK_PAGE + st->sp - 1, st->pc);
            cpu_set(mapper, STACK_PAGE + st->sp - 2, st->p | 1 << RESERVED | 1 << BREAK);
            st->sp -= 3;
            st->pc = MEM_AT16(0xFFFE);
            SET(BREAK, 1);
            cyc += 7;
            break;

        //BVC
        case 0x50: log("BVC ");
            BRANCH(! GET(OVERFLOW));
            break;

        //BVS
        case 0x70: log("BVS ");
            BRANCH(GET(OVERFLOW));
            break;

        //CLC
        case 0x18: log("CLC");
            SET(CARRY, 0);
            cyc += 2;
            break;

        //CLD
        case 0xD8: log("CLD");
            SET(DECIMAL, 0);
            cyc += 2;
            break;

        //CLI
        case 0x58: log("CLI");
            SET(INTERRUPT_DISABLE, 0);
            cyc += 2;
            break;

        //CLV
        case 0xB8: log("CLV");
            SET(OVERFLOW, 0);
            cyc += 2;
            break;

        //CMP
        case 0xC9: log("CMP ");
            OP_IMM(value1); CMP(value1); cyc += 2;
            break;
        case 0xC5: log("CMP ");
            OP_ZP(value1, 0); CMP(value1); cyc += 3;
            break;
        case 0xD5: log("CMP ");
            OP_ZP(value1, st->x); CMP(value1); cyc += 4;
            break;
        case 0xCD:  log("CMP ");
            OP_ABS(value1, 0, 0); CMP(value1); cyc += 4;
            break;
        case 0xDD: log("CMP ");
            OP_ABS(value1, st->x, 1); CMP(value1); cyc += 4;
            break;
        case 0xD9: log("CMP ");
            OP_ABS(value1, st->y, 1); CMP(value1); cyc += 4;
            break;
        case 0xC1: log("CMP ");
            OP_INDIR_PRE(value1, st->x); CMP(value1); cyc += 6;
            break;
        case 0xD1: log("CMP ");
            OP_INDIR_POS(value1, st->y, 1); CMP(value1); cyc += 5;
            break;

        //CPX
        case 0xE0: log("CPX ");
            OP_IMM(value1); CPX(value1); cyc += 2;
            break;
        case 0xE4: log("CPX ");
            OP_ZP(value1, 0); CPX(value1); cyc += 3;
            break;
        case 0xEC: log("CPX ");
            OP_ABS(value1, 0, 0); CPX(value1); cyc += 4;
            break;

        //CPY
        case 0xC0: log("CPY ");
            OP_IMM(value1); CPY(value1); cyc += 2;
            break;
        case 0xC4: log("CPY ");
            OP_ZP(value1, 0); CPY(value1); cyc += 3;
            break;
        case 0xCC: log("CPY ");
            OP_ABS(value1, 0, 0); CPY(value1); cyc += 4;
            break;

        //DEC
        case 0xC6: log("DEC ");
            OP_ZP(value1, 0); DEC(eaddr, value1); cyc += 5;
            break;
        case 0xD6: log("DEC ");
            OP_ZP(value1, st->x); DEC(eaddr, value1); cyc += 6;
            break;
        case 0xCE: log("DEC ");
            OP_ABS(value1, 0, 0); DEC(eaddr, value1); cyc += 6;
            break;
        case 0xDE: log("DEC ");
            OP_ABS(value1, st->x, 0); DEC(eaddr, value1); cyc += 7;
            break;

        //DEX
        case 0xCA: log("DEX");
            st->x--; cyc += 2;
            setZN = 1;
            result = st->x;
            break;

        //DEY
        case 0x88: log("DEY");
            st->y--; cyc += 2;
            setZN = 1;
            result = st->y;
            break;

        //EOR
        case 0x49: log("EOR ");
            OP_IMM(value1); EOR(value1); cyc += 2;
            break;
        case 0x45: log("EOR ");
            OP_ZP(value1, 0); EOR(value1); cyc += 3;
            break;
        case 0x55: log("EOR ");
            OP_ZP(value1, st->x); EOR(value1); cyc += 4;
            break;
        case 0x4D: log("EOR ");
            OP_ABS(value1, 0, 0); EOR(value1); cyc += 4;
            break;
        case 0x5D: log("EOR ");
            OP_ABS(value1, st->x, 1); EOR(value1); cyc += 4;
            break;
        case 0x59: log("EOR ");
            OP_ABS(value1, st->y, 1); EOR(value1); cyc += 4;
            break;
        case 0x41: log("EOR ");
            OP_INDIR_PRE(value1, st->x); EOR(value1); cyc += 6;
            break;
        case 0x51: log("EOR ");
            OP_INDIR_POS(value1, st->y, 1); EOR(value1); cyc += 5;
            break;

        //INC
        case 0xE6: log("INC ");
            OP_ZP(value1, 0); INC(eaddr, value1); cyc += 5;
            break;
        case 0xF6: log("INC ");
            OP_ZP(value1, st->x); INC(eaddr, value1); cyc += 6;
            break;
        case 0xEE: log("INC ");
            OP_ABS(value1, 0, 0); INC(eaddr, value1); cyc += 6;
            break;
        case 0xFE: log("INC ");
            OP_ABS(value1, st->x, 0); INC(eaddr, value1); cyc += 7;
            break;

        //INX
        case 0xE8: log("INX");
            st->x++; cyc += 2;
            result = st->x;
            setZN = 1;
            break;

        //INY
        case 0xC8: log("INY");
            st->y++; cyc += 2;
            setZN = 1;
            result = st->y;
            break;

        //JMP
        case 0x4C: log("JMP ");
            OP_ABS(value1, 0, 0); st->pc = eaddr; cyc += 3;
            break;
        case 0x6C: log("JMP ");
            OP_INDIR(addr); st->pc = addr; cyc += 5;
            break;

        //JSR
        case 0x20: log("JSR ");
            OP_ABS(value1, 0, 0);
            SET_MEM_AT16(STACK_PAGE + st->sp - 1, st->pc - 1);
            st->sp -= 2;
            st->pc = eaddr;
            cyc += 6;
            break;

        //LDA
        case 0xA9: log("LDA ");
            OP_IMM(value1); LDA(st->a, value1); cyc += 2;
            break;
        case 0xA5: log("LDA ");
            OP_ZP(value1, 0); LDA(st->a, value1); cyc += 3;
            break;
        case 0xB5: log("LDA ");
            OP_ZP(value1, st->x); LDA(st->a, value1); cyc += 4;
            break;
        case 0xAD: log("LDA ");
            OP_ABS(value1, 0, 0); LDA(st->a, value1); cyc += 4;
            break;
        case 0xBD: log("LDA ");
            OP_ABS(value1, st->x, 1); LDA(st->a, value1); cyc += 4;
            break;
        case 0xB9: log("LDA ");
            OP_ABS(value1, st->y, 1); LDA(st->a, value1); cyc += 4;
            break;
        case 0xA1: log("LDA ");
            OP_INDIR_PRE(value1, st->x); LDA(st->a, value1); cyc += 6;
            break;
        case 0xB1: log("LDA ");
            OP_INDIR_POS(value1, st->y, 1); LDA(st->a, value1); cyc += 5;
            break;

        //LDX
        case 0xA2: log("LDX ");
            OP_IMM(value1); LDA(st->x, value1); cyc += 2;
            break;
        case 0xA6: log("LDX ");
            OP_ZP(value1, 0); LDA(st->x, value1); cyc += 3;
            break;
        case 0xB6: log("LDX ");
            OP_ZP(value1, st->y); LDA(st->x, value1); cyc += 4;
            break;
        case 0xAE: log("LDX ");
            OP_ABS(value1, 0, 0); LDA(st->x, value1); cyc += 4;
            break;
        case 0xBE: log("LDX ");
            OP_ABS(value1, st->y, 1); LDA(st->x, value1); cyc += 4;
            break;

        //LDY
        case 0xA0: log("LDY ");
            OP_IMM(value1); LDA(st->y, value1); cyc += 2;
            break;
        case 0xA4: log("LDY ");
            OP_ZP(value1, 0); LDA(st->y, value1); cyc += 3;
            break;
        case 0xB4: log("LDY ");
            OP_ZP(value1, st->x); LDA(st->y, value1); cyc += 4;
            break;
        case 0xAC: log("LDY ");
            OP_ABS(value1, 0, 0); LDA(st->y, value1); cyc += 4;
            break;
        case 0xBC: log("LDY ");
            OP_ABS(value1, st->x, 1); LDA(st->y, value1); cyc += 4;
            break;

        //LSR
        case 0x4A: log("LSR A");
            REG_LSR(st->a, st->a); cyc += 2;
            break;
        case 0x46: log("LSR ");
            OP_ZP(value1, 0); LSR(eaddr, value1); cyc += 5;
            break;
        case 0x56: log("LSR ");
            OP_ZP(value1, st->x); LSR(eaddr, value1); cyc += 6;
            break;
        case 0x4E: log("LSR ");
            OP_ABS(value1, 0, 0); LSR(eaddr, value1); cyc += 6;
            break;
        case 0x5E: log("LSR ");
            OP_ABS(value1, st->x, 0); LSR(eaddr, value1); cyc += 7;
            break;

        //NOP
        case 0xEA: log("NOP");
            cyc += 2;
            break;

        //ORA
        case 0x09: log("ORA ");
            OP_IMM(value1); ORA(value1); cyc += 2;
            break;
        case 0x05: log("ORA ");
            OP_ZP(value1, 0); ORA(value1); cyc += 3;
            break;
        case 0x15: log("ORA ");
            OP_ZP(value1, st->x); ORA(value1); cyc += 4;
            break;
        case 0x0D: log("ORA ");
            OP_ABS(value1, 0, 0); ORA(value1); cyc += 4;
            break;
        case 0x1D: log("ORA ");
            OP_ABS(value1, st->x, 1); ORA(value1); cyc += 4;
            break;
        case 0x19: log("ORA ");
            OP_ABS(value1, st->y, 1); ORA(value1); cyc += 4;
            break;
        case 0x01: log("ORA ");
            OP_INDIR_PRE(value1, st->x); ORA(value1); cyc += 6;
            break;
        case 0x11: log("ORA ");
            OP_INDIR_POS(value1, st->y, 1); ORA(value1); cyc += 5;
            break;

        //PHA
        case 0x48: log("PHA");
            cpu_set(mapper, STACK_PAGE + st->sp, st->a);
            st->sp--;
            cyc += 3;
            break;

        //PHP
        case 0x08: log("PHP");
            cpu_set(mapper, STACK_PAGE + st->sp, st->p | 1 << RESERVED | 1 << BREAK);
            st->sp--;
            cyc += 3;
            break;

        //PLA
        case 0x68: log("PLA");
            setZN = 1;
            result = st->a = cpu_get(mapper, STACK_PAGE + st->sp + 1);
            st->sp++;
            cyc += 4;
            break;

        //PLP
        case 0x28: log("PLP");
            st->p = cpu_get(mapper, STACK_PAGE + st->sp + 1);
            SET(BREAK, 0);
            SET(RESERVED, 1);
            st->sp++;
            cyc += 4;
            break;

        //ROL
        case 0x2A: log("ROL A");
            REG_ROL(st->a, st->a); cyc += 2;
            break;
        case 0x26: log("ROL ");
            OP_ZP(value1, 0); ROL(eaddr, value1); cyc += 5;
            break;
        case 0x36: log("ROL ");
            OP_ZP(value1, st->x); ROL(eaddr, value1); cyc += 6;
            break;
        case 0x2E: log("ROL ");
            OP_ABS(value1, 0, 0); ROL(eaddr, value1); cyc += 6;
            break;
        case 0x3E: log("ROL ");
            OP_ABS(value1, st->x, 0); ROL(eaddr, value1); cyc += 7;
            break;

        //ROR
        case 0x6A: log("ROR A"); cyc += 2;
            REG_ROR(st->a, st->a);
            break;
        case 0x66: log("ROR ");
            OP_ZP(value1, 0); ROR(eaddr, value1); cyc += 5;
            break;
        case 0x76: log("ROR ");
            OP_ZP(value1, st->x); ROR(eaddr, value1); cyc += 6;
            break;
        case 0x6E: log("ROR ");
            OP_ABS(value1, 0, 0); ROR(eaddr, value1); cyc += 6;
            break;
        case 0x7E: log("ROR ");
            OP_ABS(value1, st->x, 0); ROR(eaddr, value1); cyc += 7;
            break;

        //RTI
        case 0x40: log("RTI");
            st->p = cpu_get(mapper, STACK_PAGE + st->sp + 1);
            SET(BREAK, 0);
            SET(RESERVED, 1);
            st->pc = MEM_AT16(STACK_PAGE + st->sp + 2);
            st->sp += 3;
            cyc += 6;
            break;

        //RTS
        case 0x60: log("RTS");
            st->pc = MEM_AT16(STACK_PAGE + st->sp + 1) + 1;
            st->sp += 2;
            cyc += 6;
            break;

        //SBC
        case 0xE9: log("SBC ");
            OP_IMM(value1); SBC(value1); cyc += 2;
            break;
        case 0xE5: log("SBC ");
            OP_ZP(value1, 0); SBC(value1); cyc += 3;
            break;
        case 0xF5: log("SBC ");
            OP_ZP(value1, st->x); SBC(value1); cyc += 4;
            break;
        case 0xED: log("SBC ");
            OP_ABS(value1, 0, 0); SBC(value1); cyc += 4;
            break;
        case 0xFD: log("SBC ");
            OP_ABS(value1, st->x, 1); SBC(value1); cyc += 4;
            break;
        case 0xF9: log("SBC ");
            OP_ABS(value1, st->y, 1); SBC(value1); cyc += 4;
            break;
        case 0xE1: log("SBC ");
            OP_INDIR_PRE(value1, st->x); SBC(value1); cyc += 6;
            break;
        case 0xF1: log("SBC ");
            OP_INDIR_POS(value1, st->y, 1); SBC(value1); cyc += 5;
            break;

        //SEC
        case 0x38: log("SEC");
            SET(CARRY, 1);
            cyc += 2;
            break;

        //SED
        case 0xF8: log("SED");
            SET(DECIMAL, 1);
            cyc += 2;
            break;

        //SEI
        case 0x78: log("SEI");
            SET(INTERRUPT_DISABLE, 1);
            cyc += 2;
            break;

        //STA
        case 0x85: log("STA ");
            OP_ZP(value1, 0); STA(eaddr); cyc += 3;
            break;
        case 0x95: log("STA ");
            OP_ZP(value1, st->x); STA(eaddr); cyc += 4;
            break;
        case 0x8D: log("STA ");
            OP_ABS(value1, 0, 0); STA(eaddr); cyc += 4;
            break;
        case 0x9D: log("STA ");
            OP_ABS(value1, st->x, 0); STA(eaddr); cyc += 5;
            break;
        case 0x99: log("STA ");
            OP_ABS(value1, st->y, 0); STA(eaddr); cyc += 5;
            break;
        case 0x81: log("STA ");
            OP_INDIR_PRE(value1, st->x); STA(eaddr); cyc += 6;
            break;
        case 0x91: log("STA ");
            OP_INDIR_POS(value1, st->y, 0); STA(eaddr); cyc += 6;
            break;

        //STX
        case 0x86: log("STX ");
            OP_ZP(value1, 0); STX(eaddr); cyc += 3;
            break;
        case 0x96: log("STX ");
            OP_ZP(value1, st->y); STX(eaddr); cyc += 4;
            break;
        case 0x8E: log("STX ");
            OP_ABS(value1, 0, 0); STX(eaddr); cyc += 4;
            break;

        //STY
        case 0x84: log("STY ");
            OP_ZP(value1, 0); STY(eaddr); cyc += 3;
            break;
        case 0x94: log("STY ");
            OP_ZP(value1, st->x); STY(eaddr); cyc += 4;
            break;
        case 0x8C: log("STY ");
            OP_ABS(value1, 0, 0); STY(eaddr); cyc += 4;
            break;

        //TAX
        case 0xAA: log("TAX");
            TRN(st->x, st->a);
            cyc += 2;
            break;

        //TAY
        case 0xA8: log("TAY");
            TRN(st->y, st->a);
            cyc += 2;
            break;

        //TSX
        case 0xBA: log("TSX");
            TRN(st->x, st->sp);
            cyc += 2;
            break;

        //TXA
        case 0x8A: log("TXA");
            TRN(st->a, st->x);
            cyc += 2;
            break;

        //TXS
        case 0x9A: log("TXS");
            TRN(st->sp, st->x);
            setZN = 0;
            cyc += 2;
            break;

        //TYA
        case 0x98: log("TYA");
            TRN(st->a, st->y);
            cyc += 2;
            break;

        default:
            log("! %02X", opcode);
    }

    if (setCV) {
        SET(CARRY, result > 255);
        SET(OVERFLOW, result > 127 || result < -128);
    }

    if (setZN) {
        SET(NEGATIVE, (result & 0x80) != 0);
        SET(ZERO, (result & 0xFF) == 0);
    }

    log("\n");

    return cyc;
}
