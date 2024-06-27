#include "Multigrid.h"

#include <chrono>

using Int = long long int;
using Real = double;

inline Int timer()
{
    static std::chrono::_V2::steady_clock::time_point time = std::chrono::steady_clock::now();
    std::chrono::_V2::steady_clock::time_point now = std::chrono::steady_clock::now();
    Int duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - time).count();
    time = now;
    return duration;
}

inline Real u(const Real &x, const Real &y)
{
    return (x - 0.5) * (x - 0.5) + (y - 0.5) * (y - 0.5);
}

inline Real alpha(const Real &x, const Real &y)
{
    return 0.5;
}

inline Real beta(const Real &x, const Real &y)
{
    return 1.0 - alpha(x, y);
}

inline Real f(const Real &x, const Real &y)
{
    return 4.0;
}

inline Real g(const Real &x, const Real &y)
{
    return alpha(x, y) * u(x, y) + beta(x, y) * 1.0;
}

int main(int argc, const char *argv[])
{
    timer();
    constexpr Int grid_size = 1024;

    mw::Poisson<Int, Real> solver(grid_size);
    solver.setBoundary(f, g, alpha, beta);
    solver.solve(std::atoll(argv[1]), std::atoll(argv[2]));
    auto res = solver.result();

    Real e = 0.0, e2 = 0.0;

    for (Int i = 0; i < res.width; i++)
    {
        for (Int j = 0; j < res.height; j++)
        {
            Real gt = u((Real)i / grid_size, (Real)j / grid_size);
            Real tmp = std::abs(res(i, j) - gt);
            e = std::max(e, tmp);
            e2 += (tmp * tmp);
        }
    }
    e2 = std::sqrt(e2 / (res.width * res.height));
    std::cout << "Error = " << e << " " << e2 << " " << ", Time = " << timer() << "(ms)." << std::endl;
    return 0;
}