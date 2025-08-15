#include <cmath>

#include "images/ImageWriter.hpp"
#include "commands/Spiral.hpp"

const double START = 1.6;
const double STEP_CORRECTION = 10;

const double MIN_SIZE = 0.075;
const double MAX_SIZE = 0.925;

double sigmoid(double t) {
    return t / std::sqrt(1 + t * t);
}

Spiral::Spiral(): Command("spiral", "Draws a spiral representing an image.", "gap, segment length, smoothness area", { 25, 1, 1100 }) {}

void Spiral::exec(Image &img, std::string &path, doubles_t &params, flags_t &flags) {
    double radius = params[0] / (2 * std::numbers::pi);
    ImageWriter result(img.width, img.height, flags);
    vec_t last_pos;
    // distance from f(t) to f(t+u) is r*sqrt(t^2+(t+u)^2-2t(t+u)cos(u))
    // assume cos(u)=1-u^2/2
    // r*u*sqrt(t(t+u)+1)=segment
    // asymptotically u~segment/r*sigmoid(t)/t, multiply by sigmoid(t/STEP_CORRECTION) for smoother start
    for (double t = START; (last_pos[0] > 0 || last_pos[1] > 0) && (last_pos[0] < img.width || last_pos[1] < img.height); t += params[1] / radius * sigmoid(t) / t * sigmoid(t / STEP_CORRECTION)) {
        vec_t pos = { std::cos(t), std::sin(t) };
        mult_arr(pos, radius * t);
        sum_arr(pos, vec_t { img.width, img.height }, 0.5);
        if (t > START) {
            double dx = std::abs(last_pos[1] - pos[1]) + 1;
            double dy = std::abs(last_pos[0] - pos[0]) + 1;
            // w*h=params[2] and w/h=dx/dy
            double w = std::sqrt(params[2] * dx / dy);
            double h = std::sqrt(params[2] * dy / dx);
            auto color = img.get_area((last_pos[0] + pos[0]) / 2 - w / 2, (last_pos[1] + pos[1]) / 2 - h / 2, w, h);
            result.draw_line(last_pos, pos, std::lerp(MAX_SIZE, MIN_SIZE, get_greyscale(color) / 255) * params[0], color);
        }
        last_pos = pos;
    }
    result.write(path);
}