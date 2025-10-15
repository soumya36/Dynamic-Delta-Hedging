#include "ResultWriter.hpp"
#include "Csv.hpp"

void writeResultCsv(const std::string& outpath, const std::vector<HedgePoint>& hp){
    std::vector<std::vector<std::string>> rows;
    rows.reserve(hp.size());
    for (auto &h: hp){
        rows.push_back({h.date, std::to_string(h.S), std::to_string(h.V), std::to_string(h.iv), std::to_string(h.delta), std::to_string(h.pnl), std::to_string(h.pnl_with_hedge)});
    }
    writeCsv(outpath, {"date","S","V","iv","delta","PNL","PNL_with_hedge"}, rows);
}
