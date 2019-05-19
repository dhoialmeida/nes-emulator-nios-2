/* operations.h

Define as operações para implementação da CPU

Variáveis externas utilizadas: (podem ser redefinidas com #define antes de incluir o header)
st = ponteiro para struct de status
result = resultado da operação (que pode ser checado no fim para setar as flags)
eaddr = endereço efetivo ("real") do último valor carregado por uma das operações OP_X
*/

// Obtém o valor de memória salvo no endereço x
// Também functiona como l-value
#define MEM_AT(x) st->memory[(x)]
// Obtém o valor de memória salvo no endereço X (16 bits)
#define MEM_AT16(x) (MEM_AT((x)+1) << 8) + MEM_AT(x)

// === Opcodes

// Armazena próximo OPCODE em var
#define OP_CODE(var) eaddr = st->pc; var = MEM_AT(eaddr); st->pc++
// Armazena valor do próximo parâmetro em var (imediato)
#define OP_IMM(var) eaddr = st->pc; var = MEM_AT(st->pc); st->pc++
// (endereçamento absoluto, com indice)
#define OP_ABS(var, idx) eaddr = MEM_AT16(st->pc) + idx; var = MEM_AT(eaddr); st->pc += 2
// (endereçamento zero page, com indice)
#define OP_ZP(var, idx) eaddr = MEM_AT(st->pc) + idx; var = MEM_AT(eaddr); st->pc++
// (enderçamento indireto, addr = 16 bits)
#define OP_INDIR(addr) eaddr = MEM_AT16(st->pc); eaddr = MEM_AT16(eaddr); addr = MEM_AT16(eaddr); st->pc += 2
// (endereçamento indireto, prefixado)
#define OP_INDIR_PRE(var, x) eaddr = MEM_AT(st->pc); eaddr = (char) (eaddr + x); eaddr = MEM_AT16(eaddr); var = MEM_AT(eaddr); st->pc += 1
// (endereçamento indireto, posfixado)
#define OP_INDIR_PRE(var, y) eaddr = MEM_AT(st->pc); eaddr = MEM_AT16(eaddr); eaddr += y; var = MEM_AT(eaddr); st->pc++;

// Salva o endereço efetivo em addr, a partir do endereço relativo rel_addr
#define LEA_REL(addr, rel_addr) (st->pc + (signed char) (rel_addr))

// === Bits de status

#define CARRY 0
#define ZERO 1
#define INTERRUPT_DISABLE 2
#define DECIMAL 3
#define OVERFLOW 6
#define NEGATIVE 7
// Seta o bit de status correspondente
#define SET(bit, value) st->p = (st->p & ~(1 << bit)) | (value << bit)
// Obtem o bit de status correspondente (0 se o bit for 0, outro valor caso contrário)
#define GET(bit) (st->p & (1 << bit))

// === Operações lógicas/aritiméticas

// Adiciona value ao acumulador
#define ADC(value) SET(CARRY, st->a > (255 - (value))); st->a += (value); result = st->a
// Efetua AND de value com o acumulador, salvando o resultado no acumulador
#define AND(value) st->a &= value; result = st->a
// Shift left one bit (dest, source)
#define ASL(dest, source) SET(CARRY, source >> 7); dest = (char) (source << 1)
