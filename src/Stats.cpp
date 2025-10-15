#include "Stats.hpp"
#include <cmath>

double pdf(double x){ static const double inv_sqrt_2pi = 0.3989422804014327; return inv_sqrt_2pi*std::exp(-0.5*x*x); }
double cdf(double x){ return 0.5 * std::erfc(-x * M_SQRT1_2); }
