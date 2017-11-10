#ifndef __OPCODE__
#define __OPCODE__

#include <stdint.h>

/* CPU instruction set and opcodes */

struct instr_t {
    uint16_t opcode;            /* CPU opcode */
    uint32_t parameters[5];     /* Up to 5 parameters per opcode */
};

#define IS_SIZE         6       /* Instruction set size */

#define OPCODE_NOP      0x00
#define OPCODE_MOV      0x01
#define OPCODE_ADD      0x02
#define OPCODE_SUB      0x03
#define OPCODE_JMP      0x04
#define OPCODE_CALL     0x05

static const uint8_t opcode_length[IS_SIZE] = {
    0,      /* NOP */
    2,      /* MOV REG, IMM */
    1,      /* ADD REG */
    1,      /* SUB REG */
    1,      /* JMP REG */
    1,      /* CALL REG */
};

#endif
