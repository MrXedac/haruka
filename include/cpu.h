#ifndef __CPU__
#define __CPU__

#include <stdint.h>

/* Special registers define */
#define CPU_REGISTER_SP 15  /* Stack pointer */
#define CPU_REGISTER_IP 14  /* Instruction pointer */

/* Main definition for CPU */
struct cpu_t {
    uint32_t cpuid; /* CPU identification */
    uint32_t regs[16]; /* 16 general registers */
    uint32_t vmr; /* MMU root configuration register */
    uint32_t ivt; /* Interrupt vector table register */
};

#endif
