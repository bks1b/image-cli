#include <stdexcept>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"

#include "images/Image.hpp"

Image::~Image() {
    stbi_image_free(data);
}

void Image::read(std::string &path) {
    if (stbi_info(path.c_str(), nullptr, nullptr, nullptr) == 0) throw std::runtime_error("Failed to read image.");
    data = stbi_load(path.c_str(), &width, &height, nullptr, 4);
    size = width * height;
}

color_t Image::get_px(int i) {
    color_t c;
    for (int j = 0; j < 3; j++) c[j] = std::lerp(255, data[4 * i + j], data[4 * i + 3] / 255.);
    return c;
}

color_t Image::get_area(int x, int y, int w, int h) {
    color_t avg = { 0, 0, 0 };
    int area = w * h;
    for (int dy = 0; dy < h; dy++) {
        for (int dx = 0; dx < w; dx++) {
            vec_t pos = { x + dx, y + dy };
            if (!in_rect(pos)) area--;
            else sum_arr(avg, get_px(get_idx(pos)));
        }
    }
    mult_arr(avg, 1. / area);
    return avg;
}

vec_t Image::get_coords(int i) {
    return { i % width, i / width };
}

int Image::get_idx(vec_t v) {
    return std::round(v[0]) + std::round(v[1]) * width;
}

bool Image::in_rect(vec_t &pos) {
    return pos[0] >= 0 && pos[1] >= 0 && std::round(pos[0]) < width && std::round(pos[1]) < height;
}