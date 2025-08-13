#pragma once

#include <string>
#include <array>
#include <vector>
#include <unordered_map>

typedef std::array<double, 3> color_t;
typedef std::array<double, 2> vec_t;
typedef std::vector<color_t> colors_t;

typedef std::vector<int> ints_t;
typedef std::vector<double> doubles_t;
typedef std::vector<ints_t> matrix_t;

typedef std::unordered_map<int, int> map_t;
typedef std::unordered_map<std::string, std::string> flags_t;

vec_t rotate(vec_t v, double t);

void add_vec(vec_t &a, vec_t b, double c);

template<typename A>
void mult_arr(A &a, double c) {
    for (auto &v : a) v *= c;
}

void rotate_line(vec_t &a, vec_t &b, double angle);

int round_by(int a, double b);

double get_greyscale(color_t c);

color_t repeat_channel(double c);

color_t parse_color(std::string &str);