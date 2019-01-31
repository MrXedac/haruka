#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "bios.h"
#include "debug.h"
#include "opcode.h"
#include "pthread.h"

#define REG(x)	vm->cpu->regs[x]
#define	IP	vm->cpu->regs[CPU_REGISTER_IP]
#define SP  vm->cpu->regs[CPU_REGISTER_SP]
#define MEM	vm->mem->memory
#define	INC_IP 	vm->cpu->regs[CPU_REGISTER_IP]++

/* Error management */
enum vm_error { OUT_OF_BOUNDS_IP, INVALID_OPCODE, DATA_ABORT };
const char* vm_errors[] = {[OUT_OF_BOUNDS_IP] = "IP out of bounds", [INVALID_OPCODE] = "Invalid opcode", [DATA_ABORT] = "Data abort"};
int errno;
extern char status[128];

void regDump(struct vm_t* vm)
{
    for(int i = 0; i < 16; i++)
    {
        dbgPrintf("R%d%s: 0x%x\n",
                i,
                (i == CPU_REGISTER_IP || 
                 i == CPU_REGISTER_SP ?
                 (i == CPU_REGISTER_IP ?
                  "(IP)":
                  "(SP)")
                 :
                 ""),
                REG(i));
    }
}

void panic(struct vm_t* vm)
{
    extern int debug;
    debug = 1;

    haruka_set_status("panic at IP 0x%x (%s) - press enter to exit", IP, vm_errors[errno]);
    /* Dump CPU registers */
    regDump(vm);

    /* Set panic flag */
    vm->panic = true;

    pthread_exit(0);
}

const opcode_handler handlers[IS_SIZE] = {
    OP(NOP),
    OP(MOV),
    OP(INC),
    OP(DEC),
    OP(JMP),
    OP(CALL),
    OP(ADD),
    OP(SUB),
    OP(PUSH),
    OP(POP),
    OP(PRT),
    OP(STOP),
    OP(MOVM),
    OP(CMP),
    OP(JEQ),
    OP(MOVR),
    OP(MOVMR),
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

OPCODE(MOVM)
{
    // dbgPrintf("MOV opcode read\n");
    uint32_t 	imm = 0x0;
    uint32_t 	val = 0x0;

    /* Destination register */
    INC_IP;
    uint8_t dst = MEM[IP];
    uint32_t dstm = vm->cpu->regs[dst];

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
    if(dstm >= MEMORY_SIZE)
    {
        dbgPrintf("Trying to write at an out-of-bounds location 0x%x\n", dstm);
        errno = DATA_ABORT;
        panic(vm);
    } else {
        MEM[dstm] = (imm >> 24) & 0xFF;
        MEM[dstm+1] = (imm >> 16) & 0xFF;
        MEM[dstm+2] = (imm >> 8) & 0xFF;
        MEM[dstm+3] = (imm) & 0xFF;
    }
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

OPCODE(MOVR)
{
    // dbgPrintf("ADD opcode read\n");
    INC_IP;
    uint8_t dst = MEM[IP];
    INC_IP;
    uint8_t src = MEM[IP];
    REG(dst) = REG(src);
    INC_IP;
}

OPCODE(MOVMR)
{
    // dbgPrintf("ADD opcode read\n");
    INC_IP;
    uint8_t dst = MEM[IP];
    uint32_t dstm = vm->cpu->regs[dst];
    INC_IP;
    uint8_t src = MEM[IP];
    uint32_t val = vm->cpu->regs[src];

    if(dstm >= MEMORY_SIZE)
    {
        dbgPrintf("Trying to write at an out-of-bounds location 0x%x\n", dstm);
        errno = DATA_ABORT;
        panic(vm);
    } else {
        MEM[dstm] = (val >> 24) & 0xFF;
        MEM[dstm+1] = (val >> 16) & 0xFF;
        MEM[dstm+2] = (val >> 8) & 0xFF;
        MEM[dstm+3] = (val) & 0xFF;
    }
    INC_IP;
}

OPCODE(CMP)
{
    // dbgPrintf("ADD opcode read\n");
    INC_IP;
    uint8_t dst = MEM[IP];
    INC_IP;
    uint8_t src = MEM[IP];
    
    /* Reset flags */
    FLAG_CLEAR(vm, FLAGS_CMP_EQ);
    FLAG_CLEAR(vm, FLAGS_CMP_LT);
    FLAG_CLEAR(vm, FLAGS_CMP_GT);

    /* Set flags */
    if(REG(dst) == REG(src))
    {
        FLAG_SET(vm, FLAGS_CMP_EQ);
    } else if (REG(dst) > REG(src)) {
        FLAG_SET(vm, FLAGS_CMP_GT);
    } else {
        FLAG_SET(vm, FLAGS_CMP_LT);
    }
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

OPCODE(JEQ)
{
    uint32_t 	adr = 0x0;
    uint32_t    val = 0x0;
    INC_IP;

    /* Get register value */
    for(int i = 0; i < 4; i++)
    {
        val = (uint32_t)MEM[IP];
        /* Shift 8 */
        adr = adr << 8;
        /* OR value */
        adr |= val;
        INC_IP;
    }
    //    dbgPrintf("Jump to %x\n", adr);
    if(vm->flags & FLAGS_CMP_EQ) {
//        dbgPrintf("JEQ fullfilled\n");
        IP = adr;
    } else { 
//        dbgPrintf("JEQ not fullfilled, not jumping to %x\n", adr);
//        dbgPrintf("IP now %x\n", IP);
    }
}

OPCODE(JMP)
{
    uint32_t 	adr = 0x0;
    uint32_t    val = 0x0;
    INC_IP;

    /* Get register value */
    for(int i = 0; i < 4; i++)
    {
        val = (uint32_t)MEM[IP];
        /* Shift 8 */
        adr = adr << 8;
        /* OR value */
        adr |= val;
        INC_IP;
    }
    //    dbgPrintf("Jump to %x\n", adr);
    IP = adr;
}

OPCODE(CALL)
{
    dbgPrintf("CALL - not impl\n");
    INC_IP;
}

OPCODE(PUSH)
{
    uint32_t sp = SP;
    uint32_t val = 0x0;
    uint32_t imm = 0x0;

    INC_IP;

    for(int i = 0; i < 4; i++)
    {
        val = (uint32_t)MEM[IP];
        imm = imm << 8;
        imm |= val;
        INC_IP;
    }

    /* Decrement stack */
    SP -= sizeof(uint32_t);

    // dbgPrintf("Pushing %x to stack %x\n", imm, SP);

    ((uint32_t*)MEM)[SP] = imm;
}

OPCODE(POP)
{
    uint32_t sp = SP;
    INC_IP;

    uint8_t dst = MEM[IP];
    INC_IP;

    REG(dst) = ((uint32_t*)MEM)[SP];
    // dbgPrintf("Pop %x into R%d\n", ((uint32_t*)MEM)[SP], dst);

    /* Increment stack */
    SP += sizeof(uint32_t);
}

OPCODE(PRT)
{
    INC_IP;
    uint32_t addr = REG(MEM[IP]);
    printf("%s", &(MEM[addr]));
    INC_IP;
}

OPCODE(STOP)
{
    INC_IP;
    vm->stopped = true;
    haruka_set_status("encountered STOP opcode at IP 0x%x, vm halted peacefully", IP);
}

void step(struct vm_t* vm)
{
    /* Do nothing if VM panics */
    if(vm->panic || vm->stopped)
        return;

    if(IP > MEMORY_SIZE)
    {
        dbgPrintf("Trying to execute code out of memory boundaries at 0x%x.\n", IP);
        errno = OUT_OF_BOUNDS_IP;
        panic(vm);
        return;
    }
    uint8_t opcode = MEM[IP];
    if(opcode < IS_SIZE){
        handlers[opcode](vm);
    }else {
        dbgPrintf("Invalid opcode %x at IP %x\n", opcode, IP);
        errno = INVALID_OPCODE;
        panic(vm);
        return;
    }
}

/* CPU cycle main function */
void* execute(void* vmx)
{
    struct vm_t* vmt = (struct vm_t*)vmx;
    vmt = initialize_machine();
    if(vmt) 
    {
        load_bios(vmt);
        extern bool initialized;
        initialized = true;
        extern struct vm_t* vm;
        vm = vmt;

        haruka_set_status("VM successfully initialized");
        /* IP is in vm->cpu->regs[CPU_REGISTER_IP] */
        while(1)
        {
            step(vmt);
        }

    }
    return NULL;
}
