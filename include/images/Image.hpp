#pragma once

#include <stdint.h>
#include <string>

#include "util/util.hpp"

class Image {
    protected:
        uint8_t *data;
    public:
        ~Image();
        void read(std::string path);
        double get_channel(int i, int c);
        color_t get_px(int i);
        color_t get_area(int x, int y, int w, int h);
        vec_t get_coords(int i);
        int get_idx(vec_t v);
        bool in_rect(vec_t pos);
        int width;
        int height;
        int size;
};