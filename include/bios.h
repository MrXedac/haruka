#ifndef __BIOS__
#define __BIOS__

#include <stdint.h>
#include "machine.h"

/* Main BIOS file */
#define BIOS_FILE	"bios.bin"

/* BIOS definitions for Haruka VM */
void load_bios(struct vm_t* vm);

#endif
