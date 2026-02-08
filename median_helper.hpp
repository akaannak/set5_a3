#pragma once
#include <vector>
#include <algorithm>

inline double median(std::vector<double> a) {
    std::size_t n = a.size();
    std::nth_element(a.begin(), a.begin() + n/2, a.end());
    double mid = a[n/2];
    if (n % 2 == 1) {
        return mid;
    }
    auto it = std::max_element(a.begin(), a.begin() + n/2);
    return (mid + *it) / 2.0;
}