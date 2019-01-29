#ifndef __HARUKA_VGA__
#define __HARUKA_VGA__

void haruka_init_vga();
void haruka_update_vga();
void haruka_close_vga();

enum vga_mode { VGA_UNINITIALIZED, VGA_TERMINAL, VGA_640_480_32 };

#endif
