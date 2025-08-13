#pragma once

#include "images/Image.hpp"

class ImageWriter : public Image {
    public:
        ImageWriter(int w, int h, flags_t &f);
        ~ImageWriter();
        void draw_line(vec_t &a, vec_t &b, double size, color_t &color);
        void fill_circle(vec_t c, double r, color_t &color);
        void write(std::string &file);
    private:
        void set_px(vec_t &pos, color_t &color, double a);
        flags_t flags;
};