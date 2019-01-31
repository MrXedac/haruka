#ifndef __MACHINE__
#define __MACHINE__

#include <stdint.h>
#include <stdbool.h>

#include "cpu.h"
#include "pmem.h"
#include "vga.h"

#define FLAGS_CMP_EQ    0x00000001
#define FLAGS_CMP_GT    0x00000002
#define FLAGS_CMP_LT    0x00000004

#define FLAG_SET(v, f)      (v->flags |= f)
#define FLAG_CLEAR(v, f)    (v->flags &= ~f)

/* Definition of the virtual machine architecture */
struct vm_t {
    struct cpu_t*   cpu;    /* CPU */
    struct pmem_t*  mem;    /* Physical memory */
    bool panic;             /* Is panicked ? */
    bool stopped;           /* Is stopped ? */
    enum vga_mode vgamode;  /* VGA mode */
    uint32_t flags;         /* CPU flags */
};

/* Machine-related methods */
struct vm_t* initialize_machine();
void shutdown_machine(struct vm_t* machine);

#endif
