#include <cmath>
#include <stdexcept>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../stb_image_write.h"

#include "images/ImageWriter.hpp"

const double GREYSCALE_MIN = 20;
const double GREYSCALE_MAX = 220;

const double SIZE_OFFSET = 1.75;
const double LINE_FADE = 3.5;

ImageWriter::ImageWriter(int w, int h, flags_t &f) {
    width = w;
    height = h;
    flags = f;
    size = w * h;
    data = (uint8_t*)malloc(3 * size);
    for (int i = 0; i < 3 * size; i++) data[i] = 255;
}

ImageWriter::~ImageWriter() {}

void ImageWriter::set_px(vec_t pos, color_t &color, double a) {
    if (!in_rect(pos)) return;
    a = std::min(1., std::max(0., a));
    auto resolved = flags.contains("color")
        ? parse_color(flags["color"])
        : flags.contains("greyscale")
            ? repeat_channel(get_greyscale(color) / 255 * (GREYSCALE_MAX - GREYSCALE_MIN) + GREYSCALE_MIN)
            : color;
    for (int c = 0; c < 3; c++) data[3 * get_idx(pos) + c] = resolved[c] * a + 255 * (1 - a);
}

void ImageWriter::draw_line(vec_t &a, vec_t &b, double size, color_t &color) {
    double dist = std::hypot(a[0] - b[0], a[1] - b[1]);
    vec_t normal = { (b[1] - a[1]) / dist, (a[0] - b[0]) / dist };
    vec_t pos = a;
    for (int i = 0; i <= std::round(dist); i++) {
        for (int j = size / 2; j >= 0; j--) {
            for (int k = -1; k < (j > 0 ? 2 : 1); k++) {
                vec_t pos_moved = pos;
                add_vec(pos_moved, normal, j * k / 2.);
                // length of projection
                double d = (a[0] - std::round(pos_moved[0])) * normal[0] + (a[1] - std::round(pos_moved[1])) * normal[1];
                set_px(pos_moved, color, ((size + SIZE_OFFSET) * (size + SIZE_OFFSET) - 4 * d * d) / (LINE_FADE * size) - 1);
            }
        }
        add_vec(pos, { -normal[1], normal[0] }, 1);
    }
}

void ImageWriter::write(std::string &path) {
    stbi_write_png(path.c_str(), width, height, 3, data, width * 3);
}