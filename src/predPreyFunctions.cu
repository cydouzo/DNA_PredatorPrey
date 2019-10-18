#include <constants.hpp>

__device__ double devLaplacien(const double* position)
{
    return (-2 * position[0] + position[1] + position[-1] )/ (dh * dh);
}

__device__ double devPreyFunction(double n, double p, double d2n)
{
    return g * n * (1 - B * g * n) - p * n - l * delta * n / (1 + p) + d2n;
}
__device__ double devPredatorFunction(double n, double p, double d2p)
{
    return n * p - delta * p / (1 + p) + d * d2p;
}
/*
    double preyFunctionTaylored(double n, double p, double d2n) const
    {
        return g * n * (1 - n / K) - (1 - delta * l) * p * n + d2n;
    }
    double predatorFunctionTaylored(double n, double p, double d2p) const
    {
        return n * p - delta * p + d * d2p;
    }
 */