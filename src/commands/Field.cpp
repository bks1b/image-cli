#include <cmath>

#include "images/ImageWriter.hpp"
#include "commands/Field.hpp"

const double RECT_SIZE = 0.6;
const double MIN_SIZE = 0.2;
const double MAX_SIZE = 1;

vec_t vector_field(double x, double y, int s) {
    // such that c-s<x+y<c+s
    int c = round_by(x + y + s, 2 * s);
    // such that (x-p)^2+(y-c+p)^2=s^2/2
    double p = (x - y + c + std::sqrt(s * s - std::pow(x + y - c, 2))) / 2;
    // tangent vector to circle
    return { p - x, y - c + p };
}

Field::Field(): Command("field", "Draws a vector field representing an image.", "size", { 10 }) {}

void Field::exec(Image &img, std::string &path, doubles_t &params, flags_t &flags) {
    ImageWriter result(round_by(img.width, params[0]), round_by(img.height, params[0]), flags);
    for (double y = params[0] / 2; y - params[0] / 2 < img.height; y += params[0]) {
        for (double x = params[0] / 2; x - params[0] / 2 < img.width; x += params[0]) {
            int s = std::max(img.width, img.height) / 4;
            auto vec = vector_field(s - x, y, s);
            mult_arr(vec, params[0] * RECT_SIZE / (2 * std::hypot(vec[0], vec[1])));
            vec_t a { x - vec[0], y - vec[1] };
            vec_t b { x + vec[0], y + vec[1] };
            auto color = img.get_area(x - params[0] / 2, y - params[0] / 2, params[0], params[0]);
            result.draw_line(a, b, std::lerp(MAX_SIZE, MIN_SIZE, get_greyscale(color) / 255) * params[0] * RECT_SIZE, color);
        }
    }
    result.write(path);
}