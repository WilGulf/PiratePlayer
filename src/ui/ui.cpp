// DEFINITIONS
#define GROOVES 5
#define GROOVE_SIZE 5
#define VINYL_RADIUS 80
#define IMAGE_RADIUS (VINYL_RADIUS - (GROOVES * GROOVE_SIZE))
//

#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "fb/fb.hpp"
#include "colors.h"

std::string image_name = "";
uint16_t image_buffer[IMAGE_RADIUS * 2 * IMAGE_RADIUS * 2];

void image_to_circle(unsigned char *image, int image_width, int image_height) {
    int target_size = IMAGE_RADIUS * 2;
    int side = std::min(image_width, image_height);
    int startx = (image_width - side) / 2;
    int starty = (image_height - side) / 2;

    int center = IMAGE_RADIUS;

    int r = IMAGE_RADIUS;
    int r2 = r * r;

    for (int y = -r; y < r; y++) {
        for (int x = -r; x < r; x++) {
            if (x * x + y * y > r2) {
                continue;
            }

            float ratio = side / target_size;
            int absolutex = x + r;
            int absolutey = y + r;

            int index = ((starty + (absolutey * ratio)) * image_width + startx + (absolutex * ratio)) * 3; // Set index to pixel in image that should be used 

            uint16_t color = ((image[index ] >> 3) << 11) |
                             ((image[index + 1] >> 2) << 5) |
                             ((image[index + 2] >> 3));

            image_buffer[(center + y) * target_size + (center + x)] = color;
        }
    }
}

void draw_vinyl(int rotation, std::string cover) {
    if (image_name != cover) {        
        int width, height, channels;
        unsigned char *image = stbi_load(cover.c_str(), &width, &height, &channels, 3);
        if (image) {
            image_to_circle(image, width, height);
            image_name = cover;
        }
        stbi_image_free(image);
    }

    draw_circle_fill(120, 120, VINYL_RADIUS, SPOTIFY_DARKGRAY);
    for (int i = 0; i < GROOVES; i++) {
        draw_circle(120, 120, VINYL_RADIUS - (i + 1) * 5, SPOTIFY_LIGHTGRAY);
    }

    draw_circle_buffer(120, 120, IMAGE_RADIUS, image_buffer, IMAGE_RADIUS * 2, rotation);

    draw_circle_fill(120, 120, 11, SPOTIFY_LIGHTGRAY);
    draw_circle_fill(120, 120, 8, SPOTIFY_GREEN);
    draw_circle_fill(120, 120, 2, 0);
}