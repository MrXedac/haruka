#ifndef __DEBUG__
#define __DEBUG__

#define dbgPrintf(a, ...) printf("DEBUG: " a, ##__VA_ARGS__)

#endif
