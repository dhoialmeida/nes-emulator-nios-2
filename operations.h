/* operations.h

Define as operações para implementação da CPU

Variáveis externas utilizadas: (podem ser redefinidas com #define antes de incluir o header)
st = ponteiro para struct de status
result = resultado da operação (que pode ser checado no fim para setar as flags), 16 bits
eaddr = endereço efetivo ("real") do último value carregado por uma das operações OP_X
setCarryOverflow = setada para falso caso o loop principal não deva atualizar CARRY/OVERFLOW
*/

// Obtém o value de memória salvo no endereço x
// Também functiona como l-value
#define MEM_AT(x) st->memory[(x)]
// Obtém o value de memória salvo no endereço X (16 bits)
#define MEM_AT16(x) (MEM_AT((x)+1) << 8) + MEM_AT(x)
// Altera o value na memória (16 bits)
#define SET_MEM_AT16(i, value) MEM_AT(i) = (value); MEM_AT(i+1) = ((value) >> 8)

// === Opcodes

// Armazena próximo OPCODE em var
#define OP_CODE(var) eaddr = st->pc; var = MEM_AT(eaddr); st->pc++
// Armazena value do próximo parâmetro em var (imediato)
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
#define OP_INDIR_PRE(var, y) eaddr = MEM_AT(st->pc); eaddr = MEM_AT16(eaddr); eaddr += y; var = MEM_AT(eaddr); st->pc++

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
#define SET(bit, value) st->p = (st->p & ~(1 << bit)) | ((value) << bit)
// Obtem o bit de status correspondente (0 se o bit for 0, outro value caso contrário)
#define GET(bit) !!(st->p & (1 << bit))
// Efetua a - b - c
#define SUB(a, b, c) (a) - (b) - (c)

// === Operações

// Adiciona value ao acumulador
#define ADC(value) result = st->a + (value) + GET(CARRY); st->a = result
// Efetua AND de value com o acumulador, salvando o resultado no acumulador
#define AND(value) st->a &= value; result = st->a
// Shift left one bit (dest, source)
#define ASL(dest, source) result = (source << 1); dest = result
// Compara value ao acumulador
#define CMP(value) result = SUB(st->a, (value), 0)
// Compara value a x
#define CPX(value) result = SUB(st->x, (value), 0)
// Compara value a y
#define CPY(value) result = SUB(st->y, (value), 0)
// Decrementa source, salva dest
#define DEC(dest, source) result = (source) - 1; dest = result
// Xor do value com acumulador, salvo em acumulador
#define EOR(value) result = st->a ^ (value); st->a = result
// Incrementa source, salva em dest
#define INC(dest, source) result = (source) + 1; dest = result
// Carrega value no dest
#define LDA(dest, source) result = source; dest = source
// Shift left one bit (dest, source)
#define LSR(dest, source) setCarryOverflow = 0; SET(CARRY, source & 0x1); result = (source >> 1); result += (source & 0x1); dest = result
// Or com acumulador
#define ORA(value) result = st->a | (value); st->a = result
// Rotate left
#define ROL(dest, value) setCarryOverflow = 0; SET(CARRY, value >> 7); dest = ((value) << 1) | ((value) >> 7); result = dest;

// Subtrai value ao acumulador, salva em dest
#define SBC(value) result = SUB(st->a, value, GET(CARRY)); st->a = result

// Transfere de registrador 1 para 2
#define TRN(dest, source) source = dest