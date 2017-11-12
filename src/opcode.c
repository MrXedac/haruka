#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "debug.h"
#include "opcode.h"

#define REG(x)	vm->cpu->regs[x]
#define	IP	vm->cpu->regs[CPU_REGISTER_IP]
#define MEM	vm->mem->memory
#define	INC_IP 	vm->cpu->regs[CPU_REGISTER_IP]++

const opcode_handler handlers[IS_SIZE] = {
	OP(NOP),
	OP(MOV),
	OP(INC),
	OP(DEC),
	OP(JMP),
	OP(CALL),
    OP(ADD),
    OP(SUB),
};

/* Do nothing and increment IP */
OPCODE(NOP)
{
//	dbgPrintf("NOP opcode read at IP %x\n", IP);
	INC_IP;
}

OPCODE(MOV)
{
	// dbgPrintf("MOV opcode read\n");
	uint32_t 	imm = 0x0;
	uint32_t 	val = 0x0;

	/* Destination register */
	INC_IP;
	uint8_t dst = MEM[IP];
	
	/* Mov 4-word value into register */
	for(int i = 0; i < 4; i++)
	{
		INC_IP;
		val = (uint32_t)MEM[IP];
		/* Shift 8 */
		imm = imm << 8;
		/* OR value */
		imm |= val;
	}
	// dbgPrintf("Value %x put into r%d\n", imm, dst);
	vm->cpu->regs[dst] = imm;
	INC_IP;
}

OPCODE(INC)
{
	// dbgPrintf("INC opcode read\n");
	INC_IP;
	uint8_t dst = MEM[IP];
	REG(dst)++;
	INC_IP;
}

OPCODE(ADD)
{
    // dbgPrintf("ADD opcode read\n");
    INC_IP;
    uint8_t dst = MEM[IP];
    INC_IP;
    uint8_t src = MEM[IP];
    REG(dst) += REG(src);
    INC_IP;
}

OPCODE(SUB)
{
    INC_IP;
    uint8_t dst = MEM[IP];
    INC_IP;
    uint8_t src = MEM[IP];
    REG(dst) -= REG(src);
    INC_IP;
}

OPCODE(DEC)
{
	// dbgPrintf("DEC opcode read\n");
	INC_IP;
	uint8_t dst = MEM[IP];
	REG(dst)--;
	INC_IP;
}

OPCODE(JMP)
{
	dbgPrintf("JMP - not impl\n");
	INC_IP;
}

OPCODE(CALL)
{
	dbgPrintf("CALL - not impl\n");
	INC_IP;
}

void regDump(struct vm_t* vm)
{
	for(int i = 0; i < 16; i++)
	{
		dbgPrintf("R%d: 0x%x\n", i, REG(i));
	}
}

void panic(struct vm_t* vm)
{
    /* Dump CPU registers */
    regDump(vm);

    /* Shutdown machine properly */
    shutdown_machine(vm);

    /* Bye ! */
    exit(-1);
}

/* CPU cycle main function */
void execute(struct vm_t* vm)
{
	/* IP is in vm->cpu->regs[CPU_REGISTER_IP] */
	while(1)
	{
        if(IP > MEMORY_SIZE)
        {
            dbgPrintf("Trying to execute code outside RAM or ROM at 0x%x.\n", IP);
            panic(vm);
            for(;;);
        }
		uint8_t opcode = MEM[IP];
		if(opcode < IS_SIZE){
			handlers[opcode](vm);
        }else {
			dbgPrintf("Invalid opcode %x at IP %x\n", opcode, IP);
			panic(vm);
			for(;;);
		}
	}
}
