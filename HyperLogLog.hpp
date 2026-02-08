#pragma once
#include <vector>
#include <cstdint>
#include <cmath>
#include <stdexcept>

class HyperLogLog {
private:
    int B;
    int m; 
    std::vector<uint8_t> M;

    static double alpha(int m) {
        if (m == 16) return 0.673;
        if (m == 32) return 0.697;
        if (m == 64) return 0.709;
        return 0.7213 / (1.0 + 1.079 / m);
    }
    static int rho(uint32_t w, int bits) {
        if (w == 0) return bits + 1;
        int lz = __builtin_clz(w);  
        int extra = 32 - bits;
        return (lz - extra) + 1;
    }

public:
    explicit HyperLogLog(int B_) : B(B_) {
        if (B < 4 || B > 16) throw std::invalid_argument("");
        m = 1 << B;
        M.assign(m, 0);
    }

    void add(uint32_t x) {
        int idx = (int)(x >> (32 - B));
        int bits = 32 - B;
        uint32_t tail;
        if (bits == 32) tail = x;
        else tail = x & ((1u << bits) - 1u);
        int r = rho(tail, bits);
        if (r > M[idx]) M[idx] = (uint8_t)r;
    }

    double estimate() const {
        double Z = 0.0;
        int V = 0;
        for (int i = 0; i < m; i++) {
            Z += std::pow(2.0, -(int)M[i]);
            if (M[i] == 0) V++;
        }

        double E = alpha(m) * (double)m * (double)m / Z;
        if (E <= 2.5 * m && V > 0) {
            E = (double)m * std::log((double)m / V);
        }
        const double TWO_32 = 4294967296.0;
        if (E > (TWO_32 / 30.0)) {
            E = -TWO_32 * std::log(1.0 - E / TWO_32);
        }
        return E;
    }
    int registers() const { return m; }
};