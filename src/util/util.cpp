#include <cmath>
#include <stdexcept>
#include <sstream>

#include "util/util.hpp"

int round_by(int a, double b) {
    return std::floor(a / b) * b;
}

vec_t rotate(vec_t v, double t) {
    return {
        v[0] * std::cos(t) - v[1] * std::sin(t),
        v[0] * std::sin(t) + v[1] * std::cos(t)
    };
}

void rotate_line(vec_t &a, vec_t &b, double angle) {
    vec_t mid = { 0, 0 };
    sum_arr(mid, a, 0.5);
    sum_arr(mid, b, 0.5);
    sum_arr(a, mid, -1);
    sum_arr(b, mid, -1);
    a = rotate(a, angle);
    b = rotate(b, angle);
    sum_arr(a, mid);
    sum_arr(b, mid);
}

double get_greyscale(color_t c) {
    return (c[0] + c[1] + c[2]) / 3.;
}

color_t repeat_channel(double c) {
    return { c, c, c };
}

color_t parse_color(std::string &str) {
    color_t color;
    if (str.size() > 0 && str[0] == '#') {
        double chars = (str.size() - 1) / 3.;
        if (chars != 1 && chars != 2) throw std::runtime_error("Invalid hex color length.");
        for (int i = 0; i < 3; i++) color[i] = std::stoi(std::string(1, str[chars * i + 1]) + str[chars * i + chars], nullptr, 16);
    } else {
        int i = 0;
        std::stringstream ss(str);
        std::string token;
        while (getline(ss, token, ',')) color[i++] = std::stod(token);
        if (i != 3) throw std::runtime_error("Invalid RGB color length.");
    }
    return color;
}