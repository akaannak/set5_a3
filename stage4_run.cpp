#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#include "RandomStreamGen.hpp"
#include "MultiHLL.hpp"
#include "ExactF0.hpp"

static double mean(const std::vector<double>& a) {
    double s = 0.0;
    for (double x : a) s += x;
    return s / (double)a.size();
}

static double stdev(const std::vector<double>& a) {
    if (a.size() < 2) return 0.0;
    double m = mean(a);
    double s2 = 0.0;
    for (double x : a) {
        double d = x - m;
        s2 += d * d;
    }
    return std::sqrt(s2 / (double)(a.size() - 1));
}

int main() {
    int N = 200000;
    int maxLen = 30;
    int B = 10;
    int K = 9;   // нечётное удобно для медианы
    int R = 50;

    std::vector<double> percents = {0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0};

    std::vector<std::vector<double>> allNt(percents.size());
    std::vector<double> trueF0(percents.size(), 0.0);

    for (int r = 0; r < R; r++) {
        RandomStreamGen gen(N, maxLen, 1000 + r);
        MultiHLL mhll(B, K, 5000 + r * 100);

        int pos = 0;
        const auto& stream = gen.getAll();

        for (std::size_t ti = 0; ti < percents.size(); ti++) {
            int need = gen.getPrefixSize(percents[ti]);

            while (pos < need) {
                mhll.add(stream[pos]);
                pos++;
            }

            auto prefix = gen.getPrefix(percents[ti]);
            std::size_t f0 = exactF0(prefix);

            double Nt = mhll.estimateMedian();
            allNt[ti].push_back(Nt);

            if (r == 0) trueF0[ti] = (double)f0;
        }
    }

    std::ofstream out("stage4_results.csv");
    out << "percent,trueF0,meanNt,stdevNt,meanMinusSigma,meanPlusSigma\n";
    for (std::size_t ti = 0; ti < percents.size(); ti++) {
        double mu = mean(allNt[ti]);
        double sd = stdev(allNt[ti]);
        out << percents[ti] << ","
            << trueF0[ti] << ","
            << mu << ","
            << sd << ","
            << (mu - sd) << ","
            << (mu + sd) << "\n";
    }
    out.close();

    std::cout << "Done. CSV saved: stage4_results.csv\n";
    std::cout << "B=" << B << " m=" << (1<<B) << " K=" << K << " R=" << R << "\n";
    return 0;
}