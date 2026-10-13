#include <cstdint>
#include <cmath>
#include <string>
#include <algorithm>

#include "font8x8.h"
#include "font6x8.h"
#include "fb.hpp"

uint16_t framebuffer[FB_HEIGHT * FB_WIDTH] = {0};
int current_font = FONT_8x8;
int char_space_h = 10;
int char_space_v = 10;

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
        std::swap(x0, x1);
        std::swap(y0, y1);
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
        std::swap(x0, x1);
        std::swap(y0, y1);
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

void draw_circle_buffer(int cx, int cy, int r, uint16_t *buffer, int buffer_line_len, int image_rotation) {
    int r2 = r * r;

    float angle = image_rotation * M_PI / 180.0f;
    float c = std::cos(angle);
    float s = std::sin(angle);

    for (int y = -r; y < r; y++) {
        for (int x = -r; x < r; x++) {
            if (x * x + y * y > r2) {
                continue;
            }
            
            if (image_rotation) {
                int sx = std::round(x * c + y * s) + r;
                int sy = std::round(-x * s + y * c) + r;

                put_pixel(cx + x, cy + y, buffer[sy * buffer_line_len + sx]);
            } else {
                put_pixel(cx + x, cy + y, buffer[(y + r) * buffer_line_len + (x + r)]);
            }
        }
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

void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color) {
    draw_line(x0, y0, x1, y1, color);
    draw_line(x1, y1, x2, y2, color);
    draw_line(x2, y2, x0, y0, color);
}

void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color) {
    float inv_slope1 = (float)(x1 - x0) / (float)(y1 - y0);
    float inv_slope2 = (float)(x2 - x0) / (float)(y2 - y0);

    float startx = x0;
    float endx = x0;

    for (int y = y0; y <= y2; y++) {
        draw_line(startx, y, endx, y, color);
        startx += inv_slope1;
        endx += inv_slope2;
    }
}

void fill_flat_top_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color) {
    float inv_slope1 = (float)(x2 - x0) / (float)(y2 - y0);
    float inv_slope2 = (float)(x2 - x1) / (float)(y2 - y1);

    float startx = x2;
    float endx = x2;

    for (int y = y2; y >= y1; y--) {
        draw_line(startx, y, endx, y, color);
        startx -= inv_slope1;
        endx -= inv_slope2;
    }
}

void draw_triangle_fill(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color) {
    if (y0 > y1) {
        std::swap(y0, y1);
        std::swap(x0, x1);
    }
    if (y1 > y2) {
        std::swap(y1, y2);
        std::swap(x1, x2);
    }
    if (y0 > y1) {
        std::swap(y0, y1);
        std::swap(x0, x1);
    }

    if (y1 == y2) {
        fill_flat_bottom_triangle(x0, y0, x1, y1, x2, y2, color);
    } else if (y0 == y1) {
        fill_flat_top_triangle(x0, y0, x1, y1, x2, y2, color);
    } else {
        int mx = ((float)((x2 - x0) * (y1 - y0)) / (float)(y2 - y0)) + x0;
        int my = y1;
        fill_flat_bottom_triangle(x0, y0, x1, y1, mx, my, color);
        fill_flat_top_triangle(x1, y1, mx, my, x2, y2, color);
    }
}

void draw_character(int x, int y, char character, uint16_t color) {
    switch (current_font) {
        case FONT_8x8:
            for (int row = 0; row < 8; row++) {
                unsigned char char_byte = console_font_8x8[((unsigned char)character * 8) + row];
                for (int col = 0; col < 8; col++) {
                    if (char_byte & (0x80 >> col)) {
                        put_pixel(x + col, y + row, color);
                    }
                }
            }
            break;
        case FONT_6x8:
            for (int row = 0; row < 8; row++) {
                unsigned char char_byte = console_font_6x8[((unsigned char)character * 8) + row];
                for (int col = 0; col < 6; col++) {
                    if (char_byte & (0x80 >> col)) {
                        put_pixel(x + col, y + row, color);
                    }
                }
            }
            break;
        default:
            return;
    }
}

void draw_string(int x, int y, std::string string, uint16_t color) {
    int n = string.length();
    int newx = x;
    int newy = y;
    for (int i = 0; i < n; i++) {
        draw_character(newx, newy, string[i], color);
        if ((newx + (char_space_h * 2)) > FB_WIDTH) {
            newx = x;
            newy += char_space_v;
        } else {
            newx += char_space_h;
        }
    }
}

void set_font(int font) {
    switch (font) {
        case FONT_8x8:
            current_font = font;
            char_space_h = 9;
            char_space_v = 10;
            break;
        case FONT_6x8:
            current_font = font;
            char_space_h = 7;
            char_space_v = 10;
            break;
        default:
            current_font = FONT_8x8;
            break;
    }
}

void clear(void) {
    std::memset(framebuffer, 0, sizeof(framebuffer));
}