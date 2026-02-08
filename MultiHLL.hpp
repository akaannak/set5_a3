#pragma once
#include <vector>
#include <string>
#include <cstdint>

#include "HyperLogLog.hpp"
#include "HashFuncGen.hpp"
#include "median_helper.hpp"

class MultiHLL {
private:
    int K;
    std::vector<HyperLogLog> sketches;
    std::vector<HashFuncGen> hashes;

public:
    MultiHLL(int B, int K_, int seedBase = 10000) : K(K_) {
        sketches.reserve(K);
        hashes.reserve(K);
        for (int i = 0; i < K; i++) {
            sketches.emplace_back(B);
            hashes.emplace_back(seedBase + i * 9973);
        }
    }

    void add(const std::string& s) {
        for (int i = 0; i < K; i++) {
            uint32_t x = hashes[i].hash(s);
            sketches[i].add(x);
        }
    }

    double estimateMedian() const {
        std::vector<double> est;
        est.reserve(K);
        for (int i = 0; i < K; i++) {
            est.push_back(sketches[i].estimate());
        }
        return median(est);
    }

    double estimateMean() const {
        double sum = 0.0;
        for (int i = 0; i < K; i++) {
            sum += sketches[i].estimate();
        }
        return sum / K;
    }
};