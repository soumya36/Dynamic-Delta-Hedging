#include "ImpliedVol.hpp"
#include "BlackScholes.hpp"
#include <limits>
#include <cmath>
#include <algorithm>

double impliedVolBisection(double S,double K,double r,double tau,double V_mkt,
                           double lo,double hi,double tol,int maxit){
    double C_intr = std::max(0.0, S - K*std::exp(-r*tau));
    if (V_mkt <= C_intr + 1e-10) return lo;
    double f_lo = callPrice(S,K,r,tau,lo) - V_mkt;
    double f_hi = callPrice(S,K,r,tau,hi) - V_mkt;
    if (f_lo*f_hi > 0){
        double h=hi;
        for(int k=0;k<20 && f_lo*f_hi>0;k++){
            h *= 1.5;
            f_hi = callPrice(S,K,r,tau,h) - V_mkt;
        }
        if (f_lo*f_hi>0){
            return std::numeric_limits<double>::quiet_NaN();
        }
        hi=h;
    }
    double a=lo, b=hi, mid=0.0;
    for(int it=0; it<maxit; ++it){
        mid = 0.5*(a+b);
        double f_mid = callPrice(S,K,r,tau,mid) - V_mkt;
        if (std::fabs(f_mid) < tol) break;
        if (f_mid * f_lo < 0){ b=mid; f_hi=f_mid; }
        else { a=mid; f_lo=f_mid; }
    }
    return std::max(lo, std::min(hi, mid));
}
