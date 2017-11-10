#ifndef __PMEM__
#define __PMEM__

#define KILOBYTE(i)     (1024 * i)
#define MEGABYTE(i)     (1024 * KILOBYTE(i))
#define GIGABYTE(i)     (1024 * MEGABYTE(i))
#define MEMORY_SIZE     MEGABYTE(16)    /* 16 Mb of physical memory */

/* Physical memory simulation */
struct pmem_t {  
    unsigned char* memory;  /* Pointer to memory space */
};

#endif
