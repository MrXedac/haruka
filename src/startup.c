#include <stdio.h>
#include <stdint.h>

#include "debug.h"
#include "machine.h"
#include "bios.h"

int main(int argc, char** argv)
{
    struct vm_t* vm;
    vm = initialize_machine();
    if(vm)
    {
        dbgPrintf("VM initialized successfully.\n");
	load_bios(vm);
	execute(vm);
	shutdown_machine(vm);
        return 0;
    } else {
        dbgPrintf("Couldn't initialize VM properly. Exiting.\n");
        return 0;
    }
}
