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

#define IS_SIZE         11       /* Instruction set size */

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

typedef void (*opcode_handler)(struct vm_t *vm);

void execute(struct vm_t *vm);

#endif
