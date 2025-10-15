#include "BlackScholes.hpp"
#include "Stats.hpp"
#include <algorithm>
#include <cmath>

double d1(double S,double K,double r,double tau,double sigma){
    if (sigma<=0.0 || tau<=0.0) return (S>K? 1e9 : -1e9);
    return (std::log(S/K) + (r + 0.5*sigma*sigma)*tau) / (sigma*std::sqrt(tau));
}
double callPrice(double S,double K,double r,double tau,double sigma){
    if (tau<=0.0) return std::max(0.0, S - K);
    double D1=d1(S,K,r,tau,sigma);
    double D2=D1 - sigma*std::sqrt(tau);
    return S*cdf(D1) - K*std::exp(-r*tau)*cdf(D2);
}
double deltaCall(double S,double K,double r,double tau,double sigma){
    if (tau<=0.0) return (S>K)? 1.0 : 0.0;
    double D1=d1(S,K,r,tau,sigma);
    return cdf(D1);
}
double vegaBS(double S,double K,double r,double tau,double sigma){
    if (tau<=0.0) return 0.0;
    double D1=d1(S,K,r,tau,sigma);
    return S*std::sqrt(tau)*pdf(D1);
}
