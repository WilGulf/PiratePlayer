#include <string>

#ifndef UI_HPP
#define UI_HPP

void draw_vinyl(int rotation, std::string cover);
void draw_static_vinyl(int cx, int cy, int r, std::string cover);

void draw_title(std::string title);
void draw_controls(bool playing, int progress);

#endif