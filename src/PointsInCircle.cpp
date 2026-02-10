#include "PointsInCircle.hpp"
#include <algorithm>

std::vector<std::pair<int, int>> PointsInCircle::getPoints(int r) {
    if (r == 0) return {{0, 0}};
    
    int ry = r;
    int rx = 2 * r; 
    
    std::vector<std::pair<int, int>> ans;
    std::vector<int> sign = {-1, 1};

    auto add_points = [&](int x, int y) {
        for(int sgx : sign) for(int sgy : sign) {
            ans.push_back({x * sgx, y * sgy});
        }
    };

    long long rx2 = (long long)rx * rx;
    long long ry2 = (long long)ry * ry;
    long long two_rx2 = 2 * rx2;
    long long two_ry2 = 2 * ry2;

    // Region 1
    long long x = 0;
    long long y = ry;
    long long p = (long long)(ry2 - rx2 * ry + 0.25 * rx2);
    long long dx = two_ry2 * x;
    long long dy = two_rx2 * y;

    while (dx < dy) {
        add_points(x, y);
        x++;
        dx += two_ry2;
        if (p < 0) {
            p += ry2 + dx;
        } else {
            y--;
            dy -= two_rx2;
            p += ry2 + dx - dy;
        }
    }

    // Region 2
    p = (long long)(ry2 * (x + 0.5) * (x + 0.5) + rx2 * (y - 1) * (y - 1) - rx2 * ry2);
    while (y >= 0) {
        add_points(x, y);
        y--;
        dy -= two_rx2;
        if (p > 0) {
            p += rx2 - dy;
        } else {
            x++;
            dx += two_ry2;
            p += rx2 - dy + dx;
        }
    }
    
    std::sort(ans.begin(), ans.end());
    ans.erase(std::unique(ans.begin(), ans.end()), ans.end());

    return ans;
} 

PointsInCircle::PointsInCircle(int _radius) {
    radius = _radius;
    
    for(int r = 0; r <= radius; r++) {
        Points.push_back(getPoints(r));           
    }
}


