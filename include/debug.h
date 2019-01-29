#ifndef __DEBUG__
#define __DEBUG__

char status[128];

#define dbgPrintf(a, ...) printf("DEBUG: " a, ##__VA_ARGS__)
#define haruka_set_status(a, ...) sprintf(status, a, ##__VA_ARGS__)

#endif
