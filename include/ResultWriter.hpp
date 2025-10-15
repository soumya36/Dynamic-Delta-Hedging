#pragma once
#include <string>
#include <vector>
#include "Hedger.hpp"

void writeResultCsv(const std::string& outpath, const std::vector<HedgePoint>& hp);
