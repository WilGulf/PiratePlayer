#include <iostream>
#include <csignal>
#include <thread>

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

    draw_controls(false, 25);

    while (1) {
        if (spin > 360) {
            spin = 0;
        }

        draw_vinyl(spin++, "test/Cover.png");
        platform_refresh();

        std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }
    
    return 0;
}