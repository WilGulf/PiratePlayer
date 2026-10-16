#include <iostream>
#include <csignal>
#include <thread>
#include <algorithm>
#include <cmath>
#include <vector>
#include <filesystem>
#include <unordered_set>

#include "platform/platform.h"
#include "ui/ui.hpp"
#include "fb/fb.hpp"
#include "mpd/mpd.h"

#define MODE_LIBRARY 1
#define MODE_PLAYER 2
int mode = MODE_LIBRARY;

#define ANIM_NO 0
#define ANIM_OPEN 1
#define ANIM_LEFT 2
#define ANIM_RIGHT 3
int curr_animation = 0;

bool playing = false;
int progress = 0;
std::string playing_name;
std::string playing_cover;

std::string musicdir = "";
std::vector<std::string> files;

std::filesystem::path get_png(std::filesystem::path string) {
    std::filesystem::path copy = string;
    copy.replace_extension(".png");
    return copy;
}

std::unordered_set<std::string> extensions = {".mp2", ".mp3", ".mp4", ".m4a", ".ogg", ".wav", ".flac"};
void reload_files(void) {
    if (!std::filesystem::exists(musicdir)) {
        return;
    }

    files.clear();
    for (const auto &entry : std::filesystem::directory_iterator(musicdir)) {
        if (entry.is_regular_file() && extensions.contains(std::filesystem::path(entry.path()).extension())) {
            files.push_back(entry.path());
        }
    }
}

void exit_player(int signal) {
    if (signal == SIGINT) {
        mpd_set_playing(true);
        mpd_clear();
        mpd_exit();
        platform_exit();
        std::exit(0);
    }
}

int spin = 0;
void mode_player(void) {
    if (playing) {
        spin++;
    }
    if (!mpd_get_playing()) {
        playing = false;
        mode = MODE_LIBRARY;
        mpd_clear();
        reload_files();
        clear();
        return;
    }
    draw_vinyl(spin, playing_cover);
    draw_title(playing_name);
    draw_controls(playing, mpd_get_progress());
    platform_refresh();
}

struct Vinyl {
    float x;
    float r;
};

struct VinylExt {
    float x;
    float r;
    std::filesystem::path file;
};

int front = 2;
Vinyl start[5];
Vinyl end[5];
VinylExt current[5];
void mode_library(void) {
    draw_static_vinyl(current[0].x, 120, current[0].r, get_png(current[0].file));
    draw_static_vinyl(current[4].x, 120, current[4].r, get_png(current[4].file));
    draw_static_vinyl(current[1].x, 120, current[1].r, get_png(current[1].file));
    draw_static_vinyl(current[3].x, 120, current[3].r, get_png(current[3].file));
    draw_static_vinyl(current[2].x, 120, current[2].r, get_png(current[2].file));
    draw_string(20, 20, current[2].file.stem(), 0xFFFF);
    platform_refresh();
}

void anim_go_right(void) {
    curr_animation = ANIM_RIGHT;
    end[4] = {180, 40};
    end[3] = {120, 80};
    end[2] = {60, 40};
    end[1] = {0, 0};
    end[0] = {0, 0};
}
void anim_go_left(void) {
    curr_animation = ANIM_LEFT;
    end[0] = {60, 40};
    end[1] = {120, 80};
    end[2] = {180, 40};
    end[3] = {240, 0};
    end[4] = {240, 0};
}
void anim_open(void) {
    curr_animation = ANIM_OPEN;
    end[0] = {0, 40};
    end[1] = {60, 40};
    end[2] = {120, 240};
    end[3] = {180, 40};
    end[4] = {240, 0};
}

int files_index = 0;

void get_prev(void) {
    if (files.empty()) {
        return;
    }

    files_index = (files_index - 1 + (int)files.size()) % (int)files.size();
}
void get_next(void) {
    if (files.empty()) {
        return;
    }
    
    files_index = (files_index + 1 + (int)files.size()) % (int)files.size();
}

int get_index(int i) {
    if (i < 0) {
        int res = (int)files.size() - std::abs(i);
        if (res < 0) {
            return get_index(res);
        } else {
            return res;
        }
    } else if (i >= (int)files.size()) {
        return (i % (int)files.size());
    }else {
        return i;
    }
}

void load_current(void) {
    if (!files.empty()) {
        for (int i = 0; i < 5; i++) {
            current[i].file = files[get_index(files_index + i - 2)];
        }
    }
}

void end_animation(void) {

    current[0] = {0, 0, current[0].file};
    current[1] = {60, 40, current[1].file};
    current[2] = {120, 80, current[2].file};
    current[3] = {180, 40, current[3].file};
    current[4] = {240, 0, current[4].file};

    if (files.empty()) {
        curr_animation = ANIM_NO;
        return;
    }

    if (curr_animation == ANIM_OPEN) {
        // ANIMATION WAS OPEN
        playing_name = current[2].file.stem().string();
        playing_cover = get_png(current[2].file).string();
        mpd_clear();
        mpd_add_pathfile(current[2].file.c_str());
        mpd_play();
        playing = true;
        mode = MODE_PLAYER;
    } else if (curr_animation == ANIM_LEFT) {
        // ANIMATION WAS LEFT
        get_next();
        load_current();
    } else if (curr_animation == ANIM_RIGHT) {
        // ANIMATION WAS RIGHT
        get_prev();
        load_current();
    }

    curr_animation = ANIM_NO;
}

int main(int argc, char **argv) {
    if (argc >= 2) {
        musicdir = argv[1];
    } else {
        std::exit(1);
    }

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

    reload_files();
    load_current();

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
                if (mode == MODE_PLAYER) {
                    mpd_set_playing(playing);
                    playing = !playing;
                }
                break;
            case DOWN: {
                if (mode == MODE_PLAYER) {
                    mode = MODE_LIBRARY;
                    mpd_set_playing(true);
                    mpd_clear();
                    reload_files();
                    clear();
                } else if (mode == MODE_LIBRARY) {
                    reload_files();
                }
                break;
            }
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