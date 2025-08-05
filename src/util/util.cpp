#include <cmath>

#include "util/util.hpp"

vec_t rotate(vec_t v, double t) {
    return {
        v[0] * std::cos(t) - v[1] * std::sin(t),
        v[0] * std::sin(t) + v[1] * std::cos(t)
    };
}

void add_vec(vec_t &a, vec_t b, double c) {
    a[0] += b[0] * c;
    a[1] += b[1] * c;
}

void rotate_line(vec_t &a, vec_t &b, double angle) {
    vec_t mid = { 0, 0 };
    add_vec(mid, a, 0.5);
    add_vec(mid, b, 0.5);
    add_vec(a, mid, -1);
    add_vec(b, mid, -1);
    a = rotate(a, angle);
    b = rotate(b, angle);
    add_vec(a, mid, 1);
    add_vec(b, mid, 1);
}

double get_greyscale(color_t c) {
    return (c[0] + c[1] + c[2]) / 3.;
}