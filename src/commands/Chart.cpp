#include "images/ImageWriter.hpp"
#include "commands/Chart.hpp"

const double STEP_X = 4.25;
const double SIZE = 2;

Chart::Chart(): Command("chart", "Draws a line chart from an image.", "step Y, height", { 10, 2 }) {}

void Chart::exec(Image &img, std::string &path, doubles_t &params, flags_t &flags) {
    ImageWriter result(round_by(img.width, STEP_X), round_by(img.height, params[0]) + params[0] * (params[1] - 1), flags);
    for (double y = params[0]; y < img.height; y += params[0]) {
        vec_t pos;
        for (double x = STEP_X; x < img.width; x += STEP_X) {
            auto color = img.get_area(x - STEP_X, y - params[0], STEP_X, params[0]);
            vec_t new_pos = { x - STEP_X / 2, y - params[0] + get_greyscale(color) / 255 * params[0] * params[1] };
            if (x > STEP_X) result.draw_line(pos, new_pos, SIZE, color);
            pos = new_pos;
        }
    }
    result.write(path);
}