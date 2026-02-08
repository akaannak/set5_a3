#pragma once
#include <string>
#include <cstdint>
#include <cstdlib>

class HashFuncGen {
private:
    uint32_t base;
    uint32_t add;

    uint32_t rndU32() {
        uint32_t x = 0;
        x ^= (uint32_t)(std::rand() & 0x7fff);
        x = (x << 15) ^ (uint32_t)(std::rand() & 0x7fff);
        x = (x << 2)  ^ (uint32_t)(std::rand() & 0x3);
        return x;
    }

public:
    HashFuncGen(int seed = 123) {
        std::srand(seed);
        base = (rndU32() | 1u);
        add  = rndU32();
    }

    uint32_t hash(const std::string& s) const {
        uint32_t h = add;
        for (unsigned char c : s) {
            h = h * base + (uint32_t)(c + 1);
        }
        h ^= (h >> 16);
        h *= 0x7feb352d;
        h ^= (h >> 15);
        h *= 0x846ca68b;
        h ^= (h >> 16);
        return h;
    }
};