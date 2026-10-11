#include <iostream>
#include <csignal>

#include "platform/platform.h"
#include "fb/fb.hpp"

void exit_player(int signal) {
    if (signal == SIGINT) {
        platform_exit();
        std::exit(0);
    }
}

int main() {
    std::signal(SIGINT, exit_player);
    std::cout << platform_init() << std::endl;

    draw_line(100, 25, 200, 75, 0xF800);
    draw_circle_fill(100, 100, 50, 0xF800);
    draw_rectangle(50, 50, 100, 150, 0x001F);
    draw_triangle(10, 10, 10, 50, 50, 50, 0x001F);
    set_font(FONT_6x8);
    draw_string(50, 10, "Hello, this is a very long string that might need more than one line", 0xFFFF);

    while (1) {
        platform_refresh();
    }
    return 0;
}