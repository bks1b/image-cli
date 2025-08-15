#include <stdexcept>
#include <cmath>
#include <numbers>
#include <random>
#include <tuple>
#include <algorithm>

#include "images/quantize.hpp"
#include "images/ImageWriter.hpp"
#include "commands/Sketch.hpp"

typedef std::vector<std::tuple<double, double, double, color_t>> lines_t;

// { angle, density, size }
// { white value, black value, cross-hatching weight, max random offset }
const double CONFIG[3][4] = {
    { std::numbers::pi / 2.5, -std::numbers::pi / 2.5, std::numbers::pi / 3, std::numbers::pi / 125 },
    { 3.75, 2.5, -0.6, 0.2 },
    { 2, 2, 0.5, 1 }
};

// cross-hatching
const double COUNT_OFFSET = 100;
const double COUNT_SCALAR = 100;

// line endpoint precision
const double T_STEP = 0.5;

const int NEIGHBORS[4][2] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } };

std::random_device rd;
std::mt19937 mt(rd());
double random_offset(const double *config) {
    std::uniform_real_distribution<double> unif(1 - config[3], 1 + config[3]);
    return unif(mt);
}

void get_lines(color_t &color, lines_t &vec) {
    double avg = get_greyscale(color);
    int count = (255 - avg + COUNT_OFFSET) / COUNT_SCALAR;
    for (int i = 0; i < count; i++) {
        double tup[3];
        for (int j = 0; j < 3; j++) {
            double v = std::lerp(CONFIG[j][0], CONFIG[j][1], avg / 255);
            tup[j] = j > 0 ? v / std::pow(count, CONFIG[j][2]) : v + CONFIG[j][2] * i / count;
        }
        vec.push_back({ tup[0], tup[1], tup[2], color });
    }
}

void get_areas(map_t &pixels, map_t &assigned, matrix_t &edges, Image &img, colors_t &colors) {
    map_t overlap;
    auto set_overlap = [&overlap](const auto &self, int x, int y) -> void {
        if (x > y) self(self, y, x);
        else if (overlap.contains(y) && x < overlap[y]) self(self, x, overlap[y]);
        else overlap[y] = x;
    };
    for (int i = 0; i < img.size; i++) {
        if (!assigned.contains(i)) {
            assigned[i] = edges.size();
            edges.push_back({});
        }
        bool edge = false;
        for (auto &d : NEIGHBORS) {
            int j = i + d[0] + d[1] * img.width;
            if (j >= 0 && j < img.size && colors[pixels[j]] == colors[pixels[i]]) {
                if (assigned.contains(j) && assigned[i] != assigned[j]) set_overlap(set_overlap, assigned[i], assigned[j]);
                else assigned[j] = assigned[i];
            } else edge = true;
        }
        if (edge) edges[assigned[i]].push_back(i);
    }
    while (true) {
        bool changed = false;
        for (int i = 0; i < img.size; i++) {
            if (overlap.contains(assigned[i])) {
                auto edge = &edges[assigned[i]];
                auto found = std::find(edge->begin(), edge->end(), i);
                if (found != edge->end()) {
                    edge->erase(found);
                    edges[overlap[assigned[i]]].push_back(i);
                }
                assigned[i] = overlap[assigned[i]];
                changed = true;
            }
        }
        if (!changed) break;
    }
}

// rotates the coordinates of each area, gets the lines' positions, then undoes the rotation
void render_sketch(map_t &pixels, map_t &assigned, matrix_t &edges, ImageWriter &result, lines_t *lines) {
    for (int g = 0; g < edges.size(); g++) {
        if (edges[g].size() == 0) continue;
        for (auto &line : lines[pixels[edges[g][0]]]) {
            double angle, density, size;
            color_t color;
            std::tie(angle, density, size, color) = line;
            std::vector<vec_t> rotated;
            for (auto &p : edges[g]) rotated.push_back(rotate(result.get_coords(p), angle));
            double bounds[2][2];
            for (int i = 0; i <= 1; i++) {
                for (int sign = -1; sign <= 1; sign += 2) {
                    bounds[i][(sign + 1) / 2] = std::max_element(rotated.begin(), rotated.end(), [sign, i](vec_t &a, vec_t &b) {
                        return a[i] * sign < b[i] * sign;
                    })->at(i);
                }
            }

            double r = (bounds[1][1] - bounds[1][0]) / density;
            for (double u = bounds[1][0] + density / 2 * (r - std::floor(r)); u <= bounds[1][1]; u += density * random_offset(CONFIG[1])) {
                bool line_start = false;
                double start_at;
                for (double t = bounds[0][0]; t <= bounds[0][1]; t += T_STEP) {
                    auto pos = rotate({ t, u }, -angle);
                    int pos_idx = result.get_idx(pos);
                    if (result.in_rect(pos) && t + T_STEP <= bounds[0][1] && assigned.contains(pos_idx) && assigned[pos_idx] == g) {
                        if (!line_start) start_at = t;
                        line_start = true;
                    } else if (line_start) {
                        line_start = false;
                        auto start_pos = rotate({ start_at, u }, -angle);
                        rotate_line(start_pos, pos, random_offset(CONFIG[0]) - 1);
                        result.draw_line(start_pos, pos, size * random_offset(CONFIG[2]), color);
                    }
                }
            }
        }
    }
}

Sketch::Sketch(): Command("sketch", "Applies a sketch effect to an image.", "color count (quantization)", { 6 }) {}

// reduces the image's palette, splits the image into areas and sketches each area
void Sketch::exec(Image &img, std::string &path, doubles_t &params, flags_t &flags) {
    int color_count = params[0];
    if (color_count < 2) throw std::runtime_error("Color count expected to be at least 2.");
    ImageWriter result(img.width, img.height, flags);
    map_t pixels;
    colors_t colors;
    lines_t lines[color_count];
    quantize(img, pixels, colors, color_count);
    for (int i = 0; i < color_count; i++) get_lines(colors[i], lines[i]);
    map_t assigned;
    matrix_t edges;
    get_areas(pixels, assigned, edges, img, colors);
    render_sketch(pixels, assigned, edges, result, lines);
    result.write(path);
}