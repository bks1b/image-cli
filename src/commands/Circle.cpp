#include "images/ImageWriter.hpp"
#include "commands/Circle.hpp"

const double MIN = 0.15;
const double MAX = 1.075;

Circle::Circle(): Command("circle", "Draws an image with circles.", "radius", { 8 }) {}

void Circle::exec(Image &img, std::string &path, doubles_t &params, flags_t &flags) {
    double r = params[0];
    ImageWriter result(round_by(img.width, 2 * r), round_by(img.height, 2 * r), flags);
    for (int y = r; y + r <= img.height; y += 2 * r) {
        for (int x = r; x + r <= img.width; x += 2 * r) {
            auto color = img.get_area(x - r, y - r, 2 * r, 2 * r);
            result.fill_circle({ x, y }, std::min(1., (1 - get_greyscale(color) / 255) * (MAX - MIN) + MIN) * r, color);
        }
    }
    result.write(path);
}