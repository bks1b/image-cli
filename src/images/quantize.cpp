#include <algorithm>
#include <cmath>

#include "images/Image.hpp"

const double CONVERGE_THRESHOLD = 8;

// k-means clustering
void quantize(Image &img, map_t &pixels, colors_t &centroids, int count) {
    for (int i = 0; i < count; i++) {
        double c = 255 * i / (count - 1);
        centroids.push_back({ c, c, c });
    }
    auto dist = [&img](color_t &a, int i) {
        auto b = img.get_px(i);
        return std::hypot(a[0] - b[0], a[1] - b[1], a[2] - b[2]);
    };
    while (true) {
        matrix_t clusters(count, ints_t {});
        for (int p = 0; p < img.size; p++) {
            clusters[std::distance(std::begin(centroids), std::min_element(centroids.begin(), centroids.end(), [p, &dist](color_t &a, color_t &b) {
                return dist(a, p) < dist(b, p);
            }))].push_back(p);
        }
        double diff = 0;
        for (int i = 0; i < count; i++) {
            color_t mean = { 0, 0, 0 };
            for (auto p : clusters[i]) sum_arr(mean, img.get_px(p), 1. / clusters[i].size());
            for (int c = 0; c < 3; c++) diff += std::abs(mean[c] - centroids[i][c]);
            centroids.push_back(mean);
        }
        if (diff / count <= CONVERGE_THRESHOLD) {
            for (int i = 0; i < count; i++) {
                for (auto p : clusters[i]) pixels.insert({ p, i });
            }
            break;
        }
        centroids.erase(centroids.begin(), centroids.begin() + count);
    }
}