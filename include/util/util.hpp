#pragma once

#include <array>
#include <vector>
#include <unordered_map>

typedef std::array<double, 3> color_t;
typedef std::array<double, 2> vec_t;
typedef std::vector<color_t> colors_t;
typedef std::unordered_map<int, int> map_t;

vec_t rotate(vec_t v, double t);

void add_vec(vec_t &a, vec_t b, double c);

void rotate_line(vec_t &a, vec_t &b, double angle);

double get_greyscale(color_t c);