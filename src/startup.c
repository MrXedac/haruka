#include <stdio.h>
#include <stdint.h>

#include "debug.h"
#include "machine.h"
#include "bios.h"
#include "vga.h"

struct vm_t* vm;

int main(int argc, char** argv)
{
    vm = initialize_machine();
    if(vm)
    {
        dbgPrintf("VM initialized successfully.\n");
        load_bios(vm);
        haruka_init_vga();
        /* execute(vm); */
        shutdown_machine(vm);
        return 0;
    } else {
        dbgPrintf("Couldn't initialize VM properly. Exiting.\n");
        return 0;
    }
}
