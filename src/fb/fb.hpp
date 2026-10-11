#include <cstdint>
#include <string>

#define FB_HEIGHT 240
#define FB_WIDTH 240

#define FONT_8x8 1
#define FONT_6x8 2
void set_font(int font);

extern uint16_t framebuffer[FB_HEIGHT * FB_WIDTH];

void draw_line(int x0, int y0, int x1, int y1, uint16_t color);
void draw_horizontal_line(int x, int len, int y, uint16_t color);
void draw_vertical_line(int x, int y, int len, uint16_t color);
void draw_circle(int cx, int cy, int r, uint16_t color);
void draw_circle_fill(int cx, int cy, int r, uint16_t color);
void draw_circle_buffer(int cx, int cy, int r, uint16_t *buffer, int buffer_line_len, int image_rotation);
void draw_rectangle(int x0, int y0, int x1, int y1, uint16_t color);
void draw_rectangle_fill(int x0, int y0, int x1, int y1, uint16_t color);
void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color);
void draw_triangle_fill(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color);
void draw_character(int x, int y, char character, uint16_t color);
void draw_string(int x, int y, std::string string, uint16_t color);