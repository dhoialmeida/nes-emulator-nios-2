#define ST_CARRY(reg, bit) reg |= (bit)

// Instrucoes e parametros
#define OPCODE(var) var = st->memory[st->pc++]
#define OP_IMM(var) var = st->memory[st->pc++]

#define OP_ABS(var, idx) var = st->memory[_(st->memory[st->pc+1] << 8 + st->memory[st->pc] + idx)]; st->pc += 2
#define OP_ZP(var, idx) var = st->memory[_(st->memory[st->pc++] + idx)]

#define ADC(value) ST_CARRY(st->p, st->a > (255 - (value))); st->a += (value)