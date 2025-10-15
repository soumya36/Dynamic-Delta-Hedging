#pragma once
#include <vector>

struct SimResult {
    std::vector<std::vector<double>> sample_paths; // up to 100 paths
    std::vector<double> terminal_HE;              // for all paths
};

SimResult simulateAndHedge(int paths, int N, double S0, double mu, double sigma, double r, double T, unsigned seed=123456);
