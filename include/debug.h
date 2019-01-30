#ifndef __DEBUG__
#define __DEBUG__

#define dbgPrintf(a, ...) { extern int debug; if(debug) printf("DEBUG: " a, ##__VA_ARGS__); }
#define haruka_set_status(a, ...) sprintf(status, a, ##__VA_ARGS__)

#endif
