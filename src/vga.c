#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vga.h"
#include "cpu.h"
#include "pmem.h"
#include "debug.h"
#include <simple2d.h>
#include <SDL2/SDL.h>
#include <pthread.h>

#define OPCODE_PER_SEC  480

S2D_Window *haruka_vga = NULL;
S2D_Image *fb = NULL;

/* Some externs - TODO : clean */
extern void step(struct vm_t* vm);
extern struct vm_t* vm;

/* Status bar text */
char status[128];

/* Updates the internals */
void vga_update_internal()
{
    // step(vm);
}

void vga_on_key(S2D_Event e) {

    switch (e.type) {
        case S2D_KEY_DOWN:
            /* Check if return was pressed */
            if(!strcmp(e.key, "Return"))
            {
                if(vm->panic || vm->stopped)
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

void vga_refresh_lfb()
{
    /* Memory for VM */
    unsigned char* mem = vm->mem->memory; 

    mem += VGA_LFB_BASE;

    /* We will process pixel by pixel */
    uint32_t* lfb = (uint32_t*)mem;

    uint32_t cur_pixel = 0;
    uint32_t max_pixel = 640 * 480;

    /* Process each pixel */
    for(uint32_t y = 0; y < 480; y++)
    {
        /* Process each line */
        for(uint32_t x = 0; x < 640; x++)
        {
            /* Find pixel value in LFB */
            uint32_t pixel = *(lfb + (640 * y) + x);
            
            uint8_t r, g, b, a;
            r = (pixel & 0xFF);
            g = ((pixel >> 8) & 0xFF);
            b = ((pixel >> 16) & 0xFF);
            a = ((pixel >> 24) & 0xFF); 
            SDL_Surface* s = fb->surface;
            
            /* Each pixel is 32-bits, B, G, R, A order */
            unsigned char* pixels = (unsigned char*)s->pixels;
            // dbgPrintf("surface at %d:%d : base 0x%x addr 0x%x max 0x%x\n", x, y, pixels, &(pixels[4 * (y * s->w + x)]), pixels + (3 * 640 * 480));
            pixels[3 * (y * s->w + x)] = r;
            pixels[3 * (y * s->w + x) + 1] = g;
            pixels[3 * (y * s->w + x) + 2] = b;
        }
    }

    /* Draw framebuffer */
    fb-> x = 0;
    fb-> y = 20;

    S2D_DrawImage(fb);
    S2D_FreeImage(fb);
    fb = S2D_CreateImage("data/blank.bmp");
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
    S2D_DrawQuad(0, 500, 1.0f, 1.0f, 1.0f, 1.0f,
            0, 515, 1.0f, 1.0f, 1.0f, 1.0f,
            639, 515, 1.0f, 1.0f, 1.0f, 1.0f,
            639, 500, 1.0f, 1.0f, 1.0f, 1.0f);

    /* Header text */
    vga_print("Haruka VM - WiP", 18, 0, 1, 0.0f, 0.0f, 0.0f, 2);
    
    if(vm->panic)
        vga_print("Crashed", 18, 1, 1, 0.0f, 0.0f, 0.0f, 1);
    else if(vm->stopped)
        vga_print("Stopped", 18, 1, 1, 0.0f, 0.0f, 0.0f, 1);
    else
        vga_print("Running", 18, 1, 1, 0.0f, 0.0f, 0.0f, 1);

    /* Footer text */
    vga_print(status, 14, 1, 501, 0.0f, 0.0f, 0.0f, 0);

    /* Is VGA initialized ? */
    if(vm->vgamode == VGA_UNINITIALIZED)
        vga_print("Guest has not initialized display yet", 14, 0, 200, 1.0f, 1.0f, 1.0f, 3);
    else
        vga_refresh_lfb();
}


/* Initializes the VGA output for Haruka VM */
void haruka_init_vga()
{
    extern bool initialized;
    while(!initialized) {};

    fb = S2D_CreateImage("data/blank.bmp");
    /* Cheat the SDL_Surface :p */
    //fb->surface->pixels = (void*)(vm->mem->memory + VGA_LFB_BASE);

    haruka_vga = S2D_CreateWindow(
            "Haruka VGA Output", 640, 515, /* vga_update_internal */0, vga_render_internal, 0
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
