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

    while (1) {
        platform_refresh();
    }
    return 0;
}