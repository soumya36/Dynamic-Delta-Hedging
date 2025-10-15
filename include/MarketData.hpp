#pragma once
#include <string>
#include <vector>
#include "Types.hpp"

Series<double> loadRiskFree(const std::string& path);
Series<double> loadAdjClose(const std::string& path);

struct OptRow { std::string date; std::string exdate; double K; char cp; double mid; };
std::vector<OptRow> loadOptions(const std::string& path);
