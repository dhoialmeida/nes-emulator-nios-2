/* operations.h

Define as operações para implementação da CPU

temp = variável de 16 bits
st = ponteiro para struct de status
*/

// Obtém o valor de memória salvo no endereço x
#define MEM_AT(x) st->memory[(x)]
// Obtém o valor de memória salvo no endereço X (16 bits)
#define MEM_AT16(x) (MEM_AT((x)+1) << 8) + MEM_AT(x)

// === Opcodes

// Armazena próximo OPCODE em var
#define OP_CODE(var) var = MEM_AT(st->pc); st->pc++
// Armazena valor do próximo parâmetro em var (imediato)
#define OP_IMM(var) var = MEM_AT(st->pc); st->pc++
// (endereçamento absoluto, com indice)
#define OP_ABS(var, idx) var = MEM_AT(MEM_AT16(st->pc) + idx); st->pc += 2
// (endereçamento zero page, com indice)
#define OP_ZP(var, idx) var = MEM_AT(MEM_AT(st->pc) + idx); st->pc++
// (enderçamento indireto, addr = 16 bits)
#define OP_INDIR(addr) temp = MEM_AT16(st->pc); temp = MEM_AT16(temp); addr = MEM_AT16(temp); st->pc += 2
// (endereçamento indireto, prefixado)
#define OP_INDIR_PRE(var, x) temp = MEM_AT(st->pc); temp = (char) (temp + x); temp = MEM_AT16(temp); var = MEM_AT(temp); st->pc += 1
// (endereçamento indireto, posfixado)
#define OP_INDIR_PRE(var, y) temp = MEM_AT(st->pc); temp = MEM_AT16(temp); temp += y; var = MEM_AT(temp); st->pc++;

// === Bits de status

// Seta o bit carry
#define SET_CARRY(reg, bit) reg |= (bit)

// === Operações lógicas/aritiméticas

// Adiciona value ao acumulador
#define ADC(value) ST_CARRY(st->p, st->a > (255 - (value))); st->a += (value)
