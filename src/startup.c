#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <string.h>
#include <argp.h>
#include <pthread.h>

#include "debug.h"
#include "machine.h"
#include "bios.h"
#include "vga.h"

struct vm_t* vm;

int debug = 0;
char biosfile[64] = BIOS_FILE;

const char *argp_program_version = "Haruka VM 0.1";
const char *argp_program_bug_address = "<quentin.bergougnoux@gmail.com>";
static char doc[] = "Haruka VM is a virtual machine for the toy, hand-made Haruka architecture.";
static char args_doc[] = "(-d|--debug) (-b|--bios [bios file])";
static struct argp_option options[] = { 
    { "debug", 'd', 0, 0, "Print debugging output. (Default: disabled)"},
    { "bios", 'b', "FILE", 0, "Use FILE as the BIOS to boot. (Default: bios.bin)"},
    { 0 } 
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    switch (key) {
        case 'd': debug = 1; break;
        case 'b': strcpy(biosfile, arg); break;
        case ARGP_KEY_ARG: return 0;
        default: return ARGP_ERR_UNKNOWN;
    }   
    return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc };

static pthread_t vm_thread;
bool initialized = false;

int main(int argc, char** argv)
{
    int c;

    /* Parse arguments */
    argp_parse (&argp, argc, argv, 0, 0, 0);

    /* Create execution thread */
    int ret = pthread_create(&vm_thread, NULL, execute, NULL);
    
    /* Execute VM */
    haruka_init_vga();

    shutdown_machine(vm);
    return 0;
}
