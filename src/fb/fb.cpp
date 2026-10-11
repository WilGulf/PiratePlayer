#include <cstdint>
#include <cmath>
#include <algorithm>

#include "fb.hpp"

uint16_t framebuffer[FB_HEIGHT * FB_WIDTH] = {0};

void put_pixel(int x, int y, uint16_t color) {
    if (x < FB_WIDTH && y < FB_HEIGHT && x >= 0 && y >= 0) {
        framebuffer[y * FB_WIDTH + x] = color;
    }
}

void draw_horizontal_line(int x, int len, int y, uint16_t color) {
    for (int i = 0; i < len; i++) {
        put_pixel(x + i, y, color);
    }
}

void draw_vertical_line(int x, int y, int len, uint16_t color) {
    for (int i = 0; i < len; i++) {
        put_pixel(x, y + i, color);
    }
}

void draw_line_h(int x0, int y0, int x1, int y1, uint16_t color) {
    if (x0 > x1) {
        x0,x1 = x1,x0;
        y0,y1 = y1,y0;
    }

    int dx = x1 - x0;
    int dy = y1 - y0;

    int dir = (dy < 0) ? -1 : 1;
    dy *= dir;

    if (dx) {
        int y = y0;
        int p = 2 * dy - dx;
        for (int i = 0; i < dx + 1; i++) {
            put_pixel(x0 + i, y, color);
            if (p >= 0) {
                y += dir;
                p -= 2 * dx;
            }

            p += 2 * dy;
        }
    }
}

void draw_line_v(int x0, int y0, int x1, int y1, uint16_t color) {
    if (y0 > y1) {
        x0,x1 = x1,x0;
        y0,y1 = y1,y0;
    }

    int dx = x1 - x0;
    int dy = y1 - y0;

    int dir = (dx < 0) ? -1 : 1;
    dx *= dir;

    if (dy) {
        int x = x0;
        int p = 2 * dx - dy;
        for (int i = 0; i < dy + 1; i++) {
            put_pixel(x, y0 + i, color);
            if (p >= 0) {
                x += dir;
                p -= 2 * dy;
            }

            p += 2 * dx;
        }
    }
}

void draw_line(int x0, int y0, int x1, int y1, uint16_t color) {
    if (std::abs(x1 - x0) > std::abs(y1 - y0)) {
        draw_line_h(x0, y0, x1, y1, color);
    } else {
        draw_line_v(x0, y0, x1, y1, color);
    }
}

void draw_circle(int cx, int cy, int r, uint16_t color) {
    int x = 0;
    int y = -r;
    int p = -r;

    while (x < -y) {
        if (p > 0) {
            y++;
            p += 2 * (x + y) + 1;
        } else {
            p += 2 * x + 1;
        }

        put_pixel(cx + x, cy + y, color);
        put_pixel(cx - x, cy + y, color);
        put_pixel(cx + x, cy - y, color);
        put_pixel(cx - x, cy - y, color);
        put_pixel(cx + y, cy + x, color);
        put_pixel(cx - y, cy + x, color);
        put_pixel(cx + y, cy - x, color);
        put_pixel(cx - y, cy - x, color);

        x++;
    }
}

void draw_circle_fill(int cx, int cy, int r, uint16_t color) {
    int x = 0;
    int y = -r;
    int p = -r;

    while (x < -y) {
        if (p > 0) {
            y++;
            p += 2 * (x + y) + 1;
        } else {
            p += 2 * x + 1;
        }

        draw_line(cx - x, cy + y, cx + x, cy + y, color);
        draw_line(cx - x, cy - y, cx + x, cy - y, color);
        draw_line(cx + y, cy + x, cx - y, cy + x, color);
        draw_line(cx + y, cy - x, cx - y, cy - x, color);

        x++;
    }
}

void draw_rectangle(int x0, int y0, int x1, int y1, uint16_t color) {
    draw_horizontal_line(x0, (x1 - x0), y0, color);
    draw_vertical_line(x0, y0, (y1 - y0), color);
    draw_horizontal_line(x0, (x1 - x0), y1, color);
    draw_vertical_line(x1, y0, (y1 - y0), color);
}

void draw_rectangle_fill(int x0, int y0, int x1, int y1, uint16_t color) {
    int lines = std::abs(y0 - y1) + 1;
    int min = std::min(y0, y1);
    for (int i = 0; i < lines; i++) {
        draw_horizontal_line(x0, (x1 - x0), min + i, color);
    }
}