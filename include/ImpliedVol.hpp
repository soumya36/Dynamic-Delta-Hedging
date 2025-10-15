#pragma once
double impliedVolBisection(double S,double K,double r,double tau,double V_mkt,
                           double lo=1e-6,double hi=5.0,double tol=1e-7,int maxit=100);
