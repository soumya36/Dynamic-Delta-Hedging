#include "MarketData.hpp"
#include "Csv.hpp"
#include <algorithm>
#include <cctype>
#include <cmath>

Series<double> loadRiskFree(const std::string &path){
    auto csv = readCsv(path);
    int id_date=-1, id_rate=-1;
    for (int i=0;i<(int)csv.header.size();++i){
        std::string h = csv.header[i];
        std::string hl=h; std::transform(hl.begin(), hl.end(), hl.begin(), ::tolower);
        if (hl.find("date")!=std::string::npos) id_date=i;
        if (hl.find("rate")!=std::string::npos) id_rate=i;
    }
    if (id_date<0 || id_rate<0) throw std::runtime_error("interest.csv missing columns");
    Series<double> s;
    for (auto &r: csv.rows){
        std::string d = r.cols[id_date];
        if (d.size()<8) continue;
        double rate_percent = std::atof(r.cols[id_rate].c_str());
        double rdec = rate_percent/100.0;
        s.dates.push_back(d);
        s.values.push_back(rdec);
    }
    s.finalize();
    return s;
}

Series<double> loadAdjClose(const std::string &path){
    auto csv=readCsv(path);
    int id_date=-1, id_close=-1;
    for (int i=0;i<(int)csv.header.size();++i){
        std::string h=csv.header[i];
        std::string hl=h; std::transform(hl.begin(),hl.end(),hl.begin(),::tolower);
        if (hl.find("date")!=std::string::npos) id_date=i;
        if (hl.find("close")!=std::string::npos && (hl.find("adj")!=std::string::npos || hl=="close")) id_close=i;
    }
    if (id_date<0 || id_close<0) throw std::runtime_error("sec_GOOG.csv missing columns");
    Series<double> s;
    for (auto &r: csv.rows){
        std::string d=r.cols[id_date];
        if (d.size()<8) continue;
        double v=std::atof(r.cols[id_close].c_str());
        s.dates.push_back(d);
        s.values.push_back(v);
    }
    s.finalize();
    return s;
}

std::vector<OptRow> loadOptions(const std::string &path){
    auto csv=readCsv(path);
    int id_date=-1,id_ex=-1,id_cp=-1,id_k=-1,id_bid=-1,id_ask=-1,id_mid=-1;
    for (int i=0;i<(int)csv.header.size();++i){
        std::string h=csv.header[i]; std::string hl=h;
        std::transform(hl.begin(),hl.end(),hl.begin(),::tolower);
        if (hl=="date" || (hl.find("date")!=std::string::npos && id_date==-1)) id_date=i;
        if (hl.find("exdate")!=std::string::npos) id_ex=i;
        if (hl.find("cp")!=std::string::npos) id_cp=i;
        if (hl.find("strk")!=std::string::npos || hl.find("strike")!=std::string::npos) id_k=i;
        if (hl.find("best_bid")!=std::string::npos || hl=="bid") id_bid=i;
        if (hl.find("best_offer")!=std::string::npos || hl=="ask") id_ask=i;
        if (hl=="mid" || hl.find("mid")!=std::string::npos) id_mid=i;
    }
    if (id_date<0 || id_ex<0 || id_cp<0 || id_k<0) throw std::runtime_error("op_GOOG.csv missing core columns");
    std::vector<OptRow> out;
    for (auto &r: csv.rows){
        std::string d=r.cols[id_date];
        if (d.size()<8) continue;
        std::string ex=r.cols[id_ex];
        std::string cp = r.cols[id_cp];
        double K = std::atof(r.cols[id_k].c_str());
        double mid;
        if (id_mid>=0){
            mid = std::atof(r.cols[id_mid].c_str());
        } else {
            double bid = id_bid>=0 ? std::atof(r.cols[id_bid].c_str()) : NAN;
            double ask = id_ask>=0 ? std::atof(r.cols[id_ask].c_str()) : NAN;
            if (std::isnan(bid) || std::isnan(ask)) continue;
            mid = 0.5*(bid+ask);
        }
        out.push_back({d,ex,K, (cp.empty()?'C':cp[0]), mid});
    }
    return out;
}
