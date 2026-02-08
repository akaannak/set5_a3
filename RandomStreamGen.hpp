#pragma once
#include <vector>
#include <string>
#include <cstdlib> 

class RandomStreamGen {
private:
    std::vector<std::string> data;
    std::string alphabet;
    int maxLen;

    int rndInt(int l, int r) {
        return l + (std::rand() % (r - l + 1));
    }

public:
    RandomStreamGen(int n, int maxLen_, int seed = 42) : maxLen(maxLen_) {
        std::srand(seed);
        for (char c = 'A'; c <= 'Z'; c++) alphabet.push_back(c);
        for (char c = 'a'; c <= 'z'; c++) alphabet.push_back(c);
        for (char c = '0'; c <= '9'; c++) alphabet.push_back(c);
        alphabet.push_back('-');
        data.reserve(n);
        for (int i = 0; i < n; i++) {
            int len = rndInt(1, maxLen);
            std::string s;
            s.reserve(len);

            for (int j = 0; j < len; j++) {
                int idx = rndInt(0, (int)alphabet.size() - 1);
                s.push_back(alphabet[idx]);
            }
            data.push_back(s);
        }
    }
    const std::vector<std::string>& getAll() const { return data; }
    int size() const { return (int)data.size(); }
    int getPrefixSize(double percent) const {
        return (int)(percent * data.size());
    }
    std::vector<std::string> getPrefix(double percent) const {
        int k = getPrefixSize(percent);
        return std::vector<std::string>(data.begin(), data.begin() + k);
    }
};