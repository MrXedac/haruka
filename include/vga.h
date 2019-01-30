#ifndef __HARUKA_VGA__
#define __HARUKA_VGA__

#define VGA_LFB_BASE    0x0C00000
#define VGA_LFB_SIZE    (640*480*4) /* 640x480px, 32 bits per pixel */

void haruka_init_vga();
void haruka_update_vga();
void haruka_close_vga();

enum vga_mode { VGA_UNINITIALIZED, VGA_TERMINAL, VGA_640_480_32 };

#endif
