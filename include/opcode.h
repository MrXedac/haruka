#ifndef __OPCODE__
#define __OPCODE__

#include <stdint.h>
#include "machine.h"

struct vm_t;

/* CPU instruction set and opcodes */

struct instr_t {
    uint16_t opcode;            /* CPU opcode */
    uint32_t parameters[5];     /* Up to 5 parameters per opcode */
};

#define IS_SIZE         17      /* Instruction set size */

#define OPCODE(x)	void handle_##x(struct vm_t *vm)
#define OP(x)		handle_##x

#define OPCODE_NOP      0x00
#define OPCODE_MOV      0x01
#define OPCODE_INC      0x02
#define OPCODE_DEC      0x03
#define OPCODE_JMP      0x04
#define OPCODE_CALL     0x05
#define OPCODE_ADD      0x06
#define OPCODE_SUB      0x07
#define OPCODE_PUSH     0x08
#define OPCODE_POP      0x09
#define OPCODE_PRT      0x0A
#define OPCODE_STOP     0x0B
#define OPCODE_MOVM     0x0C
#define OPCODE_CMP      0x0D
#define OPCODE_JEQ      0x0E
#define OPCODE_MOVR     0x0F
#define OPCODE_MOVMR    0x10

OPCODE(NOP);
OPCODE(MOV);
OPCODE(INC);
OPCODE(DEC);
OPCODE(JMP);
OPCODE(CALL);
OPCODE(ADD);
OPCODE(SUB);
OPCODE(PUSH);
OPCODE(POP);
OPCODE(PRT);
OPCODE(STOP);
OPCODE(MOVM);
OPCODE(CMP);
OPCODE(JEQ);
OPCODE(MOVR);
OPCODE(MOVMR);

typedef void (*opcode_handler)(struct vm_t *vm);

void* execute(void* vm);

#endif
