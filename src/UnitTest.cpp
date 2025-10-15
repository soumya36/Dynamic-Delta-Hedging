#include <iostream>
#include <cmath>
#include "UnitTest.hpp"
#include "BlackScholes.hpp"
#include "ImpliedVol.hpp"

// Run unit tests for option functions
void runAllTests(){
    // Test implied volatility: compute IV from a known call price and compare
    {
        double S=100, K=100, r=0.02, tau=0.5, sigma=0.2;
        double C = callPrice(S,K,r,tau,sigma);
        double iv = impliedVolBisection(S,K,r,tau,C);
        EXPECT_NEAR(iv, sigma, 1e-3);
    }

    // Test delta: check monotonicity across strikes
    {
        double r=0.02, tau=0.5, sigma=0.2;
        double d_lo = deltaCall(90,100,r,tau,sigma);
        double d_hi = deltaCall(110,100,r,tau,sigma);
        EXPECT_TRUE(d_hi > d_lo);
    }

    std::cout<<"[ALL UNIT TESTS DONE]\n";
}
