#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "machine.h"
#include "cpu.h"
#include "pmem.h"
#include "debug.h"
#include "vga.h"

struct vm_t* initialize_machine()
{
    dbgPrintf("Initializing machine with 0x%x (%dMb) memory size.\n", MEMORY_SIZE, MEMORY_SIZE/(1024*1024));
    struct vm_t* machine = (struct vm_t*)malloc(sizeof(struct vm_t));
    if(!machine)
    {
        dbgPrintf("Couldn't alloc VM.\n");
        return NULL;
    }

    dbgPrintf("Allocating CPU structure.\n");
    machine->cpu = (struct cpu_t*)malloc(sizeof(struct cpu_t));
    if(!machine->cpu) {
        dbgPrintf("Couldn't alloc CPU.\n");
        free(machine);
        return NULL;
    }
    /* BSP / Boot processor */
    machine->cpu->cpuid = 0;
    for(uint8_t i = 0; i < 16; i++)
	    machine->cpu->regs[i] = 0x0; /* Reset registers */
    machine->cpu->ivt = 0x0; /* Empty interrupt vector register */
    machine->cpu->vmr = 0x0; /* No VM root */

    dbgPrintf("Allocating PMEM structure.\n");
    machine->mem = (struct pmem_t*)malloc(sizeof(struct pmem_t));
    if(!machine->mem) {
        dbgPrintf("Couldn't alloc PMEM.\n");
        free(machine->cpu);
        free(machine);
        return NULL;
    }

    dbgPrintf("Allocating memory pool.\n");
    machine->mem->memory = (unsigned char*)malloc(MEMORY_SIZE);
    if(!machine->mem->memory){
        dbgPrintf("Couldn't alloc 0x%x bytes of PMEM.\n", MEMORY_SIZE);
        free(machine->cpu);
        free(machine->mem);
        free(machine);
        return NULL;
    }
    memset(machine->mem->memory, 0x0, MEMORY_SIZE);

    /* for(unsigned char* vga = (machine->mem->memory + VGA_LFB_BASE); vga < (machine->mem->memory + VGA_LFB_BASE + VGA_LFB_SIZE); vga=vga+4){
        *vga=0x66;
        *(vga+1)=0x99;
        *(vga+2)=0xFF;
        *(vga+3)=0xFF;
    }*/

    machine->panic = false;
    machine->vgamode = VGA_640_480_32;

    return machine;
}

void shutdown_machine(struct vm_t* machine)
{
    dbgPrintf("Shutting down VM.\n");
    free(machine->cpu);
    free(machine->mem->memory);
    free(machine->mem);
    free(machine);
    haruka_close_vga();
    return;
}
