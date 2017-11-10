#include <stdio.h>
#include <stdint.h>

#include "debug.h"
#include "machine.h"

int main(int argc, char** argv)
{
    struct vm_t* vm;
    vm = initialize_machine();
    if(vm)
    {
        dbgPrintf("VM initialized successfully.\n");
        shutdown_machine(vm);
        return 0;
    } else {
        dbgPrintf("Couldn't initialize VM properly. Exiting.\n");
        return 0;
    }
}
