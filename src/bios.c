#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "bios.h"
#include "machine.h"
#include "cpu.h"
#include "pmem.h"

void load_bios(struct vm_t* vm)
{
	FILE* bios = fopen(BIOS_FILE, "r");
	uint32_t offset = 0x0;

	while(!feof(bios))
	{
		/* Put each byte of BIOS into memory */
		vm->mem->memory[offset] = fgetc(bios);
		offset++;
	}
	
	vm->mem->memory[offset - 1] = 0x0;

	/* BIOS loaded - close file */
	fclose(bios);
}
