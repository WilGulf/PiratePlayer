#include <iostream>
#include <csignal>

#include "platform/platform.h"
#include "ui/ui.hpp"

void exit_player(int signal) {
    if (signal == SIGINT) {
        platform_exit();
        std::exit(0);
    }
}

int main() {
    std::signal(SIGINT, exit_player);
    std::cout << platform_init() << std::endl;

    int spin = 0;
    while (1) {
        draw_vinyl(spin++, "test/Cover.png");
        platform_refresh();
    }
    return 0;
}