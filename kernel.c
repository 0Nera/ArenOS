#include <kernel.h>



// Ядро ОС
int kmain(struct multiboot_info *mboot_info) {
    svga_mode_info_t *svga_mode = (svga_mode_info_t*) mboot_info->vbe_mode_info;
    
    framebuffer_addr = (uint8_t *)svga_mode->physbase; //vmm_temp_map_page(svga_mode->physbase);
    framebuffer_pitch = svga_mode->pitch;
    framebuffer_bpp = svga_mode->bpp;
    framebuffer_width = svga_mode->screen_width;
    framebuffer_height = svga_mode->screen_height;
    //framebuffer_size = framebuffer_width * framebuffer_height * (framebuffer_bpp / 8);
    framebuffer_size = framebuffer_height * framebuffer_pitch;
    back_framebuffer_addr = framebuffer_addr;

    return 0;
}