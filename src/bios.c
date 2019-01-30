#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "bios.h"
#include "machine.h"
#include "cpu.h"
#include "pmem.h"
#include "debug.h"

extern char biosfile[64];

void load_bios(struct vm_t* vm)
{
	FILE* bios = fopen(biosfile, "r");
    if(!bios)
    {
        fprintf(stderr, "Couldn't open bios file '%s'.\n", biosfile);
        exit(-1);
    }
	uint32_t offset = 0x0;

	while(!feof(bios))
	{
		/* Put each byte of BIOS into memory */
		vm->mem->memory[offset] = fgetc(bios);
		offset++;
	}
	
	vm->mem->memory[offset - 1] = 0x0;
    dbgPrintf("BIOS loaded successfully\n");
	/* BIOS loaded - close file */
	fclose(bios);
}
