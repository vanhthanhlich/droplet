#pragma once

#include <vector>
#include <utility>

class PointsInCircle {
public:
    int radius;
    std::vector<std::vector<std::pair<int, int>>> Points;

    PointsInCircle(int _radius);
    ~PointsInCircle() = default;

private:
    std::vector<std::pair<int, int>> getPoints(int r);
};
