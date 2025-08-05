#pragma once

#include "images/Image.hpp"

class ImageWriter : public Image {
    public:
        ImageWriter(int w, int h);
        ~ImageWriter();
        void set_px(vec_t pos, color_t &color, double a);
        void draw_line(vec_t &a, vec_t &b, double size, color_t &color);
        void write(const char *file);
};