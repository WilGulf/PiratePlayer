#include "fb/fb.hpp"
#include "colors.h"

#define WRAP_THRESHOLD 10

int position = 0;
std::string last_title;
void draw_title(std::string title) {
    if (title != last_title) {
        last_title = title;
        position = 0;
    }

    if (last_title.length() > WRAP_THRESHOLD) {
        position++;
        if (position >= last_title.length()) {
            position = 0;
        }
    }

    draw_string(60, 20, last_title, 0xFFFF);
}

int last_progress = 0;
void draw_controls(bool playing, int progress) {
    #define PAUSE_SIZE 15
    #define PAUSE_SIZE_HALF (PAUSE_SIZE / 2)
    #define CONTROLS_CENTER_Y 220

    #define BAR_SIZE (240 - 40)
    #define BAR_SIZE_HALF (BAR_SIZE / 2)
    #define BAR_THICKNESS 4
    #define BAR_THICKNESS_HALF (BAR_THICKNESS / 2)
    
    if (progress != last_progress || playing) {
        draw_rectangle_fill(
            120 - BAR_SIZE_HALF, CONTROLS_CENTER_Y - BAR_THICKNESS_HALF,
            120 + BAR_SIZE_HALF, CONTROLS_CENTER_Y + BAR_THICKNESS_HALF,
            SPOTIFY_WHITE
        );
        draw_rectangle_fill(
            120 - BAR_SIZE_HALF, CONTROLS_CENTER_Y - BAR_THICKNESS_HALF,
            (120 - BAR_SIZE_HALF) + (BAR_SIZE * progress / 100), CONTROLS_CENTER_Y + BAR_THICKNESS_HALF,
            SPOTIFY_GREEN
        );
    }
    last_progress = progress;

    if (playing) {
        draw_rectangle_fill(
            120 - PAUSE_SIZE_HALF, CONTROLS_CENTER_Y - PAUSE_SIZE_HALF,
            120 - PAUSE_SIZE_HALF + 4, CONTROLS_CENTER_Y + PAUSE_SIZE_HALF,
            SPOTIFY_WHITE
        );
        draw_rectangle_fill(
            120 + PAUSE_SIZE_HALF - 4, CONTROLS_CENTER_Y - PAUSE_SIZE_HALF,
            120 + PAUSE_SIZE_HALF, CONTROLS_CENTER_Y + PAUSE_SIZE_HALF,
            SPOTIFY_WHITE
        );
        draw_rectangle(
            120 - PAUSE_SIZE_HALF, CONTROLS_CENTER_Y - PAUSE_SIZE_HALF,
            120 - PAUSE_SIZE_HALF + 4, CONTROLS_CENTER_Y + PAUSE_SIZE_HALF,
            0
        );
        draw_rectangle(
            120 + PAUSE_SIZE_HALF - 4, CONTROLS_CENTER_Y - PAUSE_SIZE_HALF,
            120 + PAUSE_SIZE_HALF, CONTROLS_CENTER_Y + PAUSE_SIZE_HALF,
            0
        );
    } else {
        draw_triangle_fill(
            120 - PAUSE_SIZE_HALF, CONTROLS_CENTER_Y - PAUSE_SIZE_HALF,
            120 + PAUSE_SIZE_HALF, CONTROLS_CENTER_Y,
            120 - PAUSE_SIZE_HALF, CONTROLS_CENTER_Y + PAUSE_SIZE_HALF,
            SPOTIFY_WHITE
        );
        draw_triangle(
            120 - PAUSE_SIZE_HALF, CONTROLS_CENTER_Y - PAUSE_SIZE_HALF,
            120 + PAUSE_SIZE_HALF, CONTROLS_CENTER_Y,
            120 - PAUSE_SIZE_HALF, CONTROLS_CENTER_Y + PAUSE_SIZE_HALF,
            0
        );
    }
}