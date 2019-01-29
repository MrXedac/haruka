#ifndef __MACHINE__
#define __MACHINE__

#include <stdint.h>
#include <stdbool.h>

#include "cpu.h"
#include "pmem.h"

/* Definition of the virtual machine architecture */
struct vm_t {
    struct cpu_t*   cpu;    /* CPU */
    struct pmem_t*  mem;    /* Physical memory */
    bool panic;             /* Is panicked ? */
};

/* Machine-related methods */
struct vm_t* initialize_machine();
void shutdown_machine(struct vm_t* machine);

#endif
