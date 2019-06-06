/* operations.inc

Define as operações para implementação da CPU

Variáveis externas utilizadas:
    st = ponteiro para struct de status
    mapper = ponteiro para um Mapper
    result = resultado da operação (que pode ser checado no fim para setar as flags), 16 bits
    eaddr = endereço efetivo ("real") do último value carregado por uma das operações OP_X
    setCV = 1 se o loop principal deve atualizar flags Carry e Overflow
    setZN = 1 se o loop principal deve atualizar flags Zero e Negative
    cyc = contador de ciclos
*/

// Armazena próximo OPCODE em var
#define OP_CODE(var) eaddr = st->pc; var = cpu_get(mapper, eaddr); st->pc++

// Armazena value do próximo parâmetro em var (imediato)
#define OP_IMM(var) eaddr = st->pc; \
    var = cpu_get(mapper, st->pc); \
    log("#$%02X", var); \
    st->pc++

// (endereçamento zero page, com indice)
#define OP_ZP(var, idx) eaddr = cpu_get(mapper, st->pc); \
    log("$%02X, " #idx, eaddr); \
    eaddr = (uint8_t) (eaddr + idx); \
    var = cpu_get(mapper, eaddr); \
    st->pc++

// (endereçamento absoluto, com indice)
#define OP_ABS(var, idx, crossAdd) eaddr = MEM_AT16(mapper, st->pc); \
    log("$%04X, " #idx, eaddr); \
    result = eaddr; \
    eaddr = (eaddr + idx); \
    var = cpu_get(mapper, eaddr); \
    st->pc += 2; \
    cyc += ((result & 0xFF00) != (eaddr & 0xFF00)) & crossAdd

// (enderçamento indireto, addr = 16 bits)
#define OP_INDIR(addr) do { \
    uint16_t high, low; \
    eaddr = MEM_AT16(mapper, st->pc); \
    high = eaddr & 0xFF00; \
    low = (eaddr + 1) & 0xFF; \
    addr = cpu_get(mapper, eaddr) + (cpu_get(mapper, high + low) << 8); \
} while (0)

// (endereçamento indireto, prefixado)
#define OP_INDIR_PRE(var, x) do { \
    uint8_t tmp1; \
    eaddr = cpu_get(mapper, st->pc); \
    log("($%2X,X)", eaddr); \
    eaddr = (uint8_t) (eaddr + (x)); \
    tmp1 = (uint8_t) (eaddr + 1); \
    eaddr = cpu_get(mapper, eaddr) + (cpu_get(mapper, tmp1) << 8); \
    var = cpu_get(mapper, eaddr); \
    st->pc += 1; \
} while (0)

// (endereçamento indireto, posfixado)
#define OP_INDIR_POS(var, y, crossAdd) do { \
    uint8_t tmp1; \
    eaddr = cpu_get(mapper, st->pc); \
    log("($%2X), Y", eaddr); \
    eaddr = (uint8_t) (eaddr); \
    tmp1 = (uint8_t) (eaddr + 1); \
    result = eaddr = cpu_get(mapper, eaddr) + (cpu_get(mapper, tmp1) << 8); \
    eaddr += y; \
    var = cpu_get(mapper, eaddr); \
    st->pc += 1; \
    cyc += ((result & 0xFF00) != (eaddr & 0xFF00)) & crossAdd; \
} while (0)

// Salva o endereço efetivo em addr, a partir do endereço relativo rel_addr
#define LEA_REL(addr, rel_addr) addr = (st->pc + (signed char) (rel_addr))

// Efetua um branch relativo caso a condição seja verdadeira
// Caso a branch seja tomada, gasta 1 ciclo a mais
// Caso o destino esteja numa página diferente da página da próxima instrução,
// gasta 1 ciclo a mais.
#define BRANCH(condition) do { \
    eaddr = cpu_get(mapper, st->pc); \
    st->pc += 1; \
    result = st->pc; \
    cyc += 2; \
    log("%+03d", eaddr); \
    if (condition) { \
        cyc++; \
        LEA_REL(st->pc, eaddr); \
        cyc += ((result & 0xFF00) != (st->pc & 0xFF00)); \
    } \
} while (0)

// === Bits de status
#define CARRY 0
#define ZERO 1
#define INTERRUPT_DISABLE 2
#define DECIMAL 3
#define BREAK 4
#define RESERVED 5
#define OVERFLOW 6
#define NEGATIVE 7

// Seta o bit de status correspondente
#define SET(bit, value) st->p = (st->p & ~(1 << bit)) | ((value) << bit)
// Obtem o bit de status correspondente
#define GET(bit) !!(st->p & (1 << bit))
// Efetua a - b - c
#define SUB(a, b, c) (a) - (b) - (c)

// === Operações
// Adiciona a a b e salva o resultado em dest
#define ADC_(dest, a, b) do {\
    uint8_t tmp_a = (a); \
    uint8_t tmp_b = (b); \
    uint16_t tmp_result = tmp_a + tmp_b + GET(CARRY); \
    SET(CARRY, tmp_result > 0xFF); \
    result = ((tmp_a & 0x80) == (tmp_b & 0x80)) && ((tmp_result & 0x80) != (tmp_a & 0x80)); \
    SET(OVERFLOW, result); \
    result = tmp_result; \
    dest = result; \
    setZN = 1; \
} while (0)

// Adiciona value ao acumulador
#define ADC(value) ADC_(st->a, st->a, value)

// Efetua AND de value com o acumulador, salvando o resultado no acumulador
#define AND(value) setZN = 1; st->a &= value; result = st->a

// Shift left one bit (reg, source)
#define REG_ASL(dest, source) setZN = 1; SET(CARRY, source >> 7); result = (source << 1); dest = result;
// Shift left one bit (M, source)
#define ASL(dest, source) setZN = 1; SET(CARRY, source >> 7); result = (source << 1); cpu_set(mapper, dest, result)

// Efeuta a operação BIT
#define BIT(value) result = st->a & value; \
    SET(ZERO, result == 0); \
    SET(OVERFLOW, (value & 64) >> 6); \
    SET(NEGATIVE, (value & 128) >> 7)

// Compara A com B e seta as flags
#define COMPARE(a, b) do { \
    uint8_t tmp_a = (a); \
    uint8_t tmp_b = (b); \
    SET(CARRY, tmp_a >= tmp_b); \
    SET(ZERO, tmp_a == tmp_b); \
    SET(NEGATIVE, (tmp_a - tmp_b) >> 7); \
} while (0)

// Compara value ao acumulador
#define CMP(value) COMPARE(st->a, value)

// Compara value a x
#define CPX(value) COMPARE(st->x, value)

// Compara value a y
#define CPY(value) COMPARE(st->y, value)

// Decrementa source, salva em dest (MEM)
#define DEC(dest, source) setZN = 1; result = (source) - 1; cpu_set(mapper, dest, result)

// Xor do value com acumulador, salvo em acumulador
#define EOR(value) setZN = 1; result = st->a ^ (value); st->a = result

// Incrementa source, salva em dest (MEM)
#define INC(dest, source) setZN = 1; result = (source) + 1; cpu_set(mapper, dest, result)

// Carrega value no dest
#define LDA(dest, source) setZN = 1; dest = source; result = dest

// Shift right one bit (dest, source)
#define REG_LSR(dest, source) setCV = 0; setZN = 1; SET(CARRY, source & 0x1); result = (source >> 1); dest = result
// Shift right one bit (dest, source)
#define LSR(dest, source) setCV = 0; setZN = 1; SET(CARRY, source & 0x1); result = (source >> 1); cpu_set(mapper, dest, result)

// Or com acumulador
#define ORA(value) setZN = 1; result = st->a | (value); st->a = result

// Rotate left (REG)
#define REG_ROL(dest, value) setCV = 0; setZN = 1; result = (value >> 7) & 1; dest = ((value) << 1) | GET(CARRY); SET(CARRY, result); result = dest
// Rotate left (MEM)
#define ROL(dest, value) setCV = 0; setZN = 1; result = ((value) << 1) | GET(CARRY); cpu_set(mapper, dest, result); SET(CARRY, (value >> 7) & 1)

// Rotate right (REG)
#define REG_ROR(dest, value) setCV = 0; setZN = 1; result = (value & 1); dest = ((value) >> 1) | (GET(CARRY) << 7); SET(CARRY, result); result = dest
// Rotate right (MEM)
#define ROR(dest, value) setCV = 0; setZN = 1; result = ((value) >> 1) | (GET(CARRY) << 7); cpu_set(mapper, dest, result); SET(CARRY, value & 1)

// Subtrai value ao acumulador
#define SBC(value) ADC_(st->a, st->a, ~value)

// Salva acumulador na memória
#define STA(addr) cpu_set(mapper, eaddr, st->a)

// Salva X na memória
#define STX(addr) cpu_set(mapper, eaddr, st->x)

// Salva Y na memória
#define STY(addr) cpu_set(mapper, eaddr, st->y)

// Transfere de registrador 1 para 2
#define TRN(dest, source) setZN = 1; dest = source; result = dest
