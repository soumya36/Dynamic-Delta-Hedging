#include "Hedger.hpp"
#include "Date.hpp"
#include "ImpliedVol.hpp"
#include "BlackScholes.hpp"
#include <unordered_map>
#include <cmath>

std::vector<HedgePoint> runHedgeSeries(
    const std::string& t0, const std::string& tN, const std::string& exdate, double K,
    const Series<double>& S, const Series<double>& r, const std::vector<OptRow>& opts)
{
    std::unordered_map<std::string,double> Vmid;
    for (const auto& o: opts){
        if (o.cp!='C') continue;
        if (std::fabs(o.K - K) > 1e-9) continue;
        if (o.exdate != exdate) continue;
        Vmid[o.date] = o.mid;
    }
    auto alldates = enumerateDates(t0, tN);
    std::vector<std::string> dates;
    for (auto &d: alldates){
        auto ymd = parseYMD(d);
        int dow = weekday(ymd.y, ymd.m, ymd.d);
        if (dow>=1 && dow<=5){
            if (S.idx.find(d)!=S.idx.end() && r.idx.find(d)!=r.idx.end() && Vmid.find(d)!=Vmid.end()){
                dates.push_back(d);
            }
        }
    }
    if (dates.empty()) throw std::runtime_error("No overlapping dates with all series");

    std::vector<HedgePoint> out;
    out.reserve(dates.size());

    std::string d0 = dates.front();
    double S0 = S.at(d0);
    double r0 = r.at(d0);
    double V0 = Vmid[d0];
    double tau0 = std::max(0, countWeekdays(d0, exdate)) / 252.0;
    double iv0 = impliedVolBisection(S0, K, r0, tau0, V0);
    double delta0 = deltaCall(S0, K, r0, tau0, iv0);
    double B_prev = V0 - delta0 * S0;
    double delta_prev = delta0;

    out.push_back({d0, S0, V0, iv0, delta0, 0.0, 0.0});

    for (size_t i=1;i<dates.size();++i){
        std::string di = dates[i];
        std::string dim1 = dates[i-1];
        double Si = S.at(di);
        double rim1 = r.at(dim1);
        double Vi = Vmid[di];
        double taui = std::max(0, countWeekdays(di, exdate)) / 252.0;
        double ivi = impliedVolBisection(Si, K, r.at(di), taui, Vi);
        double deltai = deltaCall(Si, K, r.at(di), taui, ivi);
        double growth = std::exp(rim1 * (1.0/252.0));
        double Bi = delta_prev * Si + B_prev * growth - deltai * Si;
        double HEi = delta_prev * Si + B_prev * growth - Vi;
        double PNLi = V0 - Vi;
        double PNLH = HEi;
        out.push_back({di, Si, Vi, ivi, deltai, PNLi, PNLH});
        delta_prev = deltai;
        B_prev = Bi;
    }
    return out;
}
