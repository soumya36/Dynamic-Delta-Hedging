#include "Simulator.hpp"
#include "BlackScholes.hpp"
#include <random>
#include <algorithm>
#include <cmath>

SimResult simulateAndHedge(int paths, int N, double S0, double mu, double sigma, double r, double T, unsigned seed){
    double dt = T / N;
    std::mt19937_64 rng(seed);
    std::normal_distribution<double> Z(0.0,1.0);
    SimResult R;
    int record_paths = std::min(paths, 100);

    for (int p=0;p<paths;++p){
        std::vector<double> Sseries(N+1);
        Sseries[0]=S0;
        for (int i=0;i<N;i++){
            double St = Sseries[i];
            double dW = std::sqrt(dt)*Z(rng);
            double next = St + mu*St*dt + sigma*St*dW;
            Sseries[i+1]=std::max(1e-12, next);
        }
        if (p<record_paths) R.sample_paths.push_back(Sseries);

        double tau = T;
        double S0p = Sseries[0];
        double K = 105.0;
        double V0 = callPrice(S0p, K, r, tau, sigma);
        double delta0 = deltaCall(S0p, K, r, tau, sigma);
        double Bprev = V0 - delta0*S0p;
        double deltaprev = delta0;
        double HE=0.0;
        for (int i=1;i<=N;i++){
            double Si = Sseries[i];
            double rim1 = r;
            double taui = std::max(0.0, T - i*dt);
            double Vi = callPrice(Si, K, r, taui, sigma);
            double deltai = deltaCall(Si, K, r, taui, sigma);
            double growth = std::exp(rim1 * dt);
            double Bi = deltaprev * Si + Bprev * growth - deltai * Si;
            double HEi = deltaprev * Si + Bprev * growth - Vi;
            HE = HEi;
            deltaprev = deltai;
            Bprev = Bi;
        }
        R.terminal_HE.push_back(HE);
    }
    return R;
}
