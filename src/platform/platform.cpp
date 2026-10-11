#include "platform/platform.h"

int platform_init(void) {
    return sdl_init();
}

void platform_refresh(void) {
    sdl_refresh();
}

int platform_exit(void) {
    return sdl_exit();
}