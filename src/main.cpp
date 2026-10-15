#include <iostream>
#include <csignal>
#include <thread>
#include <algorithm>
#include <cmath>

#include "platform/platform.h"
#include "ui/ui.hpp"
#include "fb/fb.hpp"
#include "mpd/mpd.h"

#define MODE_LIBRARY 1
#define MODE_PLAYER 2
int mode = MODE_LIBRARY;

void exit_player(int signal) {
    if (signal == SIGINT) {
        platform_exit();
        mpd_exit();
        std::exit(0);
    }
}

int spin = 0;
void mode_player(void) {
    draw_vinyl(spin++, "test/Cover.png");
    draw_title("Let It Happen Let It Happen");
    draw_controls(false, 25);
    platform_refresh();
}

struct Vinyl {
    float x;
    float r;
};

struct VinylExt {
    float x;
    float r;
    std::string cover;
};

int front = 2;
Vinyl start[5];
Vinyl end[5];
VinylExt current[5];
void mode_library(void) {
    draw_static_vinyl(current[0].x, 120, current[0].r, current[0].cover);
    draw_static_vinyl(current[4].x, 120, current[4].r, current[4].cover);
    draw_static_vinyl(current[1].x, 120, current[1].r, current[1].cover);
    draw_static_vinyl(current[3].x, 120, current[3].r, current[3].cover);
    draw_static_vinyl(current[2].x, 120, current[2].r, current[2].cover);
    platform_refresh();
}

void mode_settings(void) {

}

void anim_go_right(void) {
    end[4] = {180, 40};
    end[3] = {120, 80};
    end[2] = {60, 40};
    end[1] = {0, 0};
    end[0] = {0, 0};
}
void anim_go_left(void) {
    end[0] = {60, 40};
    end[1] = {120, 80};
    end[2] = {180, 40};
    end[3] = {240, 0};
    end[4] = {240, 0};
}
void anim_open(void) {
    end[0] = {0, 40};
    end[1] = {60, 40};
    end[2] = {120, 240};
    end[3] = {180, 40};
    end[4] = {240, 0};
}

void end_animation(void) {
    bool left = false;
    if (!end[0].r) {
        left = true;
    }

    if (end[2].r > 80) {
        // ANIMATION WAS OPEN
        mode = MODE_PLAYER;
    } else if (left) {
        // ANIMATION WAS LEFT
        int i = 0;
        for (0; i < 4; i++) {
            current[i].cover = current[i + 1].cover;
        }
        current[4].cover = "test/Cover.png";
    } else {
        // ANIMATION WAS RIGHT
        int i = 4;
        for (; i > 0; i--) {
            current[i].cover = current[i - 1].cover;
        }
        current[0].cover = "test/Cover.png";
    }

    current[0] = {0, 0, current[0].cover};
    current[1] = {60, 40, current[1].cover};
    current[2] = {120, 80, current[2].cover};
    current[3] = {180, 40, current[3].cover};
    current[4] = {240, 0, current[4].cover};
}

int main() {
    std::signal(SIGINT, exit_player);
    std::cout << platform_init() << std::endl;
    std::cout << mpd_init() << std::endl;

    start[0] = {0, 0};
    start[1] = {60, 40};
    start[2] = {120, 80};
    start[3] = {180, 40};
    start[4] = {240, 0};

    for (int i = 0; i < 5; i++) {
        current[i] = {start[i].x, start[i].r, ""};
    }

    float time = 0;
    bool animating = false;
    while (1) {
        switch (platform_poll()) {
            case LEFT:
                if (!animating && mode == MODE_LIBRARY) {
                    anim_go_left();
                    animating = true;
                }
                break;
            case RIGHT:
                if (!animating && mode == MODE_LIBRARY) {
                    anim_go_right();
                    animating = true;
                }
                break;
            case UP:
                if (!animating && mode == MODE_LIBRARY) {
                    anim_open();
                    animating = true;
                }
                break;
            case DOWN:
                break;
        }

        if (spin > 360) {
            spin = 0;
        }

        if (animating) {
            clear();

            for (int i = 0; i < 5; i++) {
                current[i].x = std::lerp(start[i].x, end[i].x, time);
                current[i].r = std::lerp(start[i].r, end[i].r, time);
            }

            time = std::min(time + 0.025f / 0.4f, 1.0f);
            if (time >= 1.0f) {
                animating = false;
                end_animation();
                time = 0;
            }
        }

        switch (mode) {
            case MODE_PLAYER:
                mode_player();
                break;
            case MODE_LIBRARY:
                mode_library();
                break;
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }

    platform_exit();
    mpd_exit();
    
    return 0;
}