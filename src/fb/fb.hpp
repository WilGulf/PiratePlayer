#include <cstdint>

#define FB_HEIGHT 240
#define FB_WIDTH 240

extern uint16_t framebuffer[FB_HEIGHT * FB_WIDTH];

void draw_line(int x0, int y0, int x1, int y1, uint16_t color);
void draw_circle(int cx, int cy, int r, uint16_t color);
void draw_circle_fill(int cx, int cy, int r, uint16_t color);
void draw_rectangle(int x0, int y0, int x1, int y1, uint16_t color);
void draw_rectangle_fill(int x0, int y0, int x1, int y1, uint16_t color);