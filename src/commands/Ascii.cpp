#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>
#include <cmath>
#include <numeric>

#include "images/ImageWriter.hpp"
#include "commands/Ascii.hpp"

typedef std::vector<std::pair<int, std::vector<double>>> data_t;

const char *TEMP = "temp";
const char *FILENAME = "ascii.txt";

const int START[2] = { 10, 9 };
const int END[2] = { 22, 25 };
const int W = 4;
const int H = 8;

const char *URL = "https://raw.githubusercontent.com/nchambers/unicodeviz/refs/heads/master/imgs/";
const std::string CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789'\"+!%/=()~^`\\|[]$<>#&@{};*,.-?:_ ";

const std::string file_path = std::string(TEMP) + "/" + FILENAME;

// saves the normalized average area colors of each char in the list
data_t create_data() {
    data_t data;
    std::filesystem::create_directory(TEMP);
    for (auto c : CHARS) {
        std::stringstream ss;
        ss << std::hex << int(c);
        std::string id = ss.str() + ".png";
        std::string path = TEMP + ("/" + id);
        if (id.length() < 9) id.insert(0, 9 - id.length(), '0');
        if (!std::filesystem::exists(path)) std::system(("curl " + (URL + id + " -o " + path)).c_str());
        Image img;
        img.read(path);
        std::vector<double> areas;
        double min = 255;
        double step_y = (END[1] - START[1]) / H;
        double step_x = (END[0] - START[0]) / W;
        for (int y = START[1]; y < END[1]; y += step_y) {
            for (int x = START[0]; x < END[0]; x += step_x) {
                double v = get_greyscale(img.get_area(x, y, step_x, step_y));
                areas.push_back(v);
                if (v < min) min = v;
            }
        }
        if (min < 255) for (auto &x : areas) x = (x - min) / (255 - min) * 255;
        data.push_back({ int(c), areas });
    }
    std::ofstream file(file_path);
    file << CHARS << "\n" << START[0] << "," << START[1] << "," << END[0] << "," << END[1] << ",\n";
    for (auto &entry : data) {
        file << std::get<0>(entry) << ",";
        for (auto c : std::get<1>(entry)) file << c << ",";
        file << "\n";
    }
    file.close();
    return data;
}

// reads data from the local file, or rewrites it if it doesn't exist or is outdated
data_t get_data() {
    if (!std::filesystem::exists(file_path)) return create_data();
    std::ifstream file(file_path);
    data_t data;
    std::string line;
    for (int i = 0; std::getline(file, line); i++) {
        if (i == 0) {
            if (line != CHARS) {
                file.close();
                return create_data();
            }
            continue;
        }
        int id = i == 1 ? -1 : 0;
        std::vector<double> vec;
        std::stringstream ss;
        ss << line;
        std::string num;
        while (std::getline(ss, num, ',')) {
            if (id == 0) id = std::stoi(num);
            else vec.push_back(std::stod(num));
        }
        if (i == 1) {
            if (vec != std::vector<double> { START[0], START[1], END[0], END[1] }) {
                file.close();
                return create_data();
            }
        } else data.push_back({ id, vec });
    }
    file.close();
    return data;
}

Ascii::Ascii(): Command("ascii", "Generates ASCII art from an image.", "shrink X, shrink Y, brightness weight", { 1, 1, 1.5 }) {}

// finds the best match to the chars of each area of the image
void Ascii::exec(Image &img, const char *path, std::vector<double> args) {
    int shrink_x = args[0];
    int shrink_y = args[1];
    auto data = get_data();
    std::vector<double> sums;
    for (auto &x : data) {
        auto vec = std::get<1>(x);
        sums.push_back(std::accumulate(vec.begin(), vec.end(), 0., [](double a, double b) {
            return a + b;
        }));
    }
    std::ofstream file(path);
    for (int y = 0; y + H * shrink_y - 1 < img.height; y += H * shrink_y) {
        for (int x = 0; x + W * shrink_x - 1 < img.width; x += W * shrink_x) {
            std::vector<double> area;
            double area_min;
            double area_max;
            double area_sum = 0;
            for (int dy = 0; dy < H; dy++) {
                for (int dx = 0; dx < W; dx++) {
                    double v = get_greyscale(img.get_area(x + dx * shrink_x, y + dy * shrink_y, shrink_x, shrink_y));
                    area.push_back(v);
                    area_sum += v;
                    if (dx + dy == 0 || v < area_min) area_min = v;
                    if (dx + dy == 0 || v > area_max) area_max = v;
                }
            }
            auto min_it = data.begin();
            double min_val;
            for (auto it = data.begin(); it != data.end(); it++) {
                // average color match
                double val = args[2] * std::abs(area_sum - sums[std::distance(data.begin(), it)]) / 255;
                // normalized area by area match
                for (int i = 0; i < W * H; i++) val += std::abs((area[i] - area_min + 1) / (area_max - area_min + 1) - std::get<1>(*it)[i] / 255);
                int min_dist = std::distance(min_it, it);
                if (min_dist == 0 || val < min_val) {
                    min_val = val;
                    min_it += min_dist;
                }
            }
            file << char(get<0>(*min_it));
        }
        file << "\n";
    }
    file.close();
}