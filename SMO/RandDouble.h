#ifndef RANDDOUBLE_H
#define RANDDOUBLE_H

#include <functional>
#include <random>

class Rand_double
{
public:
    Rand_double(double low = 0, double high = 1)
    :r(std::bind(std::uniform_real_distribution<>(low,high),std::default_random_engine())){}

    double operator()(){ return r(); }

private:
    std::function<double()> r;
};


#endif // RANDDOUBLE_H
