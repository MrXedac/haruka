#include <stdio.h>
#include <stdlib.h>

#include "machine.h"
#include "cpu.h"
#include "pmem.h"
#include "debug.h"

struct vm_t* initialize_machine()
{
    dbgPrintf("Initializing machine with 0x%x memory size.\n", MEMORY_SIZE);
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

    return machine;
}

void shutdown_machine(struct vm_t* machine)
{
    dbgPrintf("Shutting down VM.\n");
    free(machine->cpu);
    free(machine->mem->memory);
    free(machine->mem);
    free(machine);
    return;
}
