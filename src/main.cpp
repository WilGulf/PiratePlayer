#include <iostream>
#include <csignal>
#include <thread>

#include "platform/platform.h"
#include "ui/ui.hpp"
#include "mpd/mpd.h"

void exit_player(int signal) {
    if (signal == SIGINT) {
        platform_exit();
        mpd_exit();
        std::exit(0);
    }
}

int spin = 0;
void mode_playing(void) {
    draw_vinyl(spin++, "test/Cover.png");
    draw_controls(false, 25);
    platform_refresh();
}

void mode_library(void) {

}

void mode_settings(void) {

}

int main() {
    std::signal(SIGINT, exit_player);
    std::cout << platform_init() << std::endl;
    std::cout << mpd_init() << std::endl;

    while (1) {
        if (spin > 360) {
            spin = 0;
        }

        mode_playing();
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }

    platform_exit();
    mpd_exit();
    
    return 0;
}