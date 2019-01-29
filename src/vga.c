#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vga.h"
#include "cpu.h"
#include "pmem.h"
#include "debug.h"
#include <simple2d.h>

#define OPCODE_PER_SEC  480

S2D_Window *haruka_vga = NULL;

/* Some externs - TODO : clean */
extern void step(struct vm_t* vm);
extern struct vm_t* vm;

/* Status bar text */
//char status[128];

/* Updates the internals */
void vga_update_internal()
{
    step(vm);
}

void vga_on_key(S2D_Event e) {

    switch (e.type) {
        case S2D_KEY_DOWN:
            /* Check if return was pressed */
            if(!strcmp(e.key, "Return"))
            {
                if(vm->panic)
                {
                    shutdown_machine(vm);
                    exit(0);
                }
            }
            break;

        case S2D_KEY_HELD:
            break;

        case S2D_KEY_UP:
            break;
    }
}

void vga_print(char* str, int weight, int x, int y, float r, float g, float b, int align)
{
    S2D_Text* txt = S2D_CreateText("data/terminus.ttf", str, weight);
    if(!align || align > 3) {
        txt->x = x;
    } else if (align == 1) {
        txt->x = 1;
    } else if (align == 2) {
        txt->x = 639 - txt->width - 1;
    } else if (align == 3) {
        txt->x = (640/2) - 1 - ((txt->width/2) - 1);
    }
    txt->y = y;
    txt->color.r = r;
    txt->color.g = g;
    txt->color.b = b;
    txt->color.a = 1.0f;

    S2D_DrawText(txt);
    S2D_FreeText(txt);
}

/* Renders screen */
void vga_render_internal()
{
    /* Draw header */
    S2D_DrawQuad(0, 0, 1.0f, 1.0f, 1.0f, 1.0f,
            0, 20, 1.0f, 1.0f, 1.0f, 1.0f,
            639, 20, 1.0f, 1.0f, 1.0f, 1.0f,
            639, 0, 1.0f, 1.0f, 1.0f, 1.0f);

    /* Draw footer */
    S2D_DrawQuad(0, 505, 1.0f, 1.0f, 1.0f, 1.0f,
            0, 520, 1.0f, 1.0f, 1.0f, 1.0f,
            639, 520, 1.0f, 1.0f, 1.0f, 1.0f,
            639, 505, 1.0f, 1.0f, 1.0f, 1.0f);

    /* Header text */
    vga_print("Haruka VM - WiP", 18, 0, 1, 0.0f, 0.0f, 0.0f, 2);
    if(vm->panic)
        vga_print("Stopped", 18, 1, 1, 0.0f, 0.0f, 0.0f, 1);
    else
        vga_print("Running", 18, 1, 1, 0.0f, 0.0f, 0.0f, 1);

    /* Footer text */
    vga_print(status, 14, 1, 506, 0.0f, 0.0f, 0.0f, 0);

    /* Is VGA initialized ? */
    if(vm->vgamode == VGA_UNINITIALIZED)
        vga_print("Guest has not initialized display yet", 14, 0, 200, 1.0f, 1.0f, 1.0f, 3);
}


/* Initializes the VGA output for Haruka VM */
void haruka_init_vga()
{
    haruka_set_status("Haruka VGA output successfully initialized");
    haruka_vga = S2D_CreateWindow(
            "Haruka VGA Output", 640, 520, vga_update_internal, vga_render_internal, 0
            );

    haruka_vga->vsync = true;
    haruka_vga->fps_cap = OPCODE_PER_SEC;
    haruka_vga->on_key = vga_on_key;
    S2D_Show(haruka_vga);
}


/* Releases VGA resources */
void haruka_close_vga()
{
}
