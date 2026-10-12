#include "platform/platform.h"

int platform_init(void) {
    #ifdef DESKTOP_SDL
    return sdl_init();
    #elif defined(PI_ST7789)
    return -1;
    #endif
}

void platform_refresh(void) {
    #ifdef DESKTOP_SDL
    sdl_refresh();
    #elif defined(PI_ST7789)
    return -1;
    #endif
}

int platform_exit(void) {
    #ifdef DESKTOP_SDL
    return sdl_exit();
    #elif defined(PI_ST7789)
    return -1;
    #endif
}