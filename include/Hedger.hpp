#pragma once
#include <string>
#include <vector>
#include "Types.hpp"
#include "MarketData.hpp"

struct HedgePoint {
    std::string date;
    double S;
    double V;
    double iv;
    double delta;
    double pnl;
    double pnl_with_hedge;
};

std::vector<HedgePoint> runHedgeSeries(
    const std::string& t0, const std::string& tN, const std::string& exdate, double K,
    const Series<double>& S, const Series<double>& r, const std::vector<OptRow>& opts);
