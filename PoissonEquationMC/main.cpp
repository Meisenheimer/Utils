#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <numeric>
#include <cmath>
#include <cassert>

#include <MTK/Random.h>

using Int = long long int;
using Real = double;

inline constexpr Int REPEAT_TIME = 1024;
inline constexpr Int GRID_SIZE = 128;
inline constexpr Real DX = 1.0 / (Real)GRID_SIZE;

inline Real u(const Real &x)
{
    return (x - 0.5) * (x - 0.5);
}

inline Real f(const Real &x)
{
    return 2.0;
}

inline Real MC(const Int &begin)
{
    Int index = begin;
    mtk::Random random;
    Real res = 0.0;
    while (index != 0 && index != GRID_SIZE)
    {
        res -= (DX * DX * f((Real)index / (Real)GRID_SIZE) / 2.0);
        if (random.uniform<Real>(0, 1) >= 0.5)
        {
            index++;
        }
        else
        {
            index--;
        }
    }
    res += u((Real)index / (Real)GRID_SIZE);
    return res;
}

inline std::vector<Real> solver()
{
    std::vector<Real> res(GRID_SIZE + 1, 0);
#pragma omp parallel for
    for (Int i = 0; i <= GRID_SIZE; i++)
    {
        for (Int n = 0; n < REPEAT_TIME; n++)
        {
            res.at(i) += MC(i);
        }
        res.at(i) /= (Real)REPEAT_TIME;
    }
    return res;
}

inline Int timer()
{
    static std::chrono::_V2::steady_clock::time_point time = std::chrono::steady_clock::now();
    std::chrono::_V2::steady_clock::time_point now = std::chrono::steady_clock::now();
    Int duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - time).count();
    time = now;
    return duration;
}

int main()
{
    timer();
    auto res = solver();
    Real e = 0.0, e2 = 0.0;
    for (Int i = 0; i <= GRID_SIZE; i++)
    {
        Real x = (Real)i / GRID_SIZE;
        Real gt = u(x);
        Real tmp = std::abs(res.at(i) - gt);
        e = std::max(e, tmp);
        e2 += (tmp * tmp);
    }
    e2 = std::sqrt(e2 / (GRID_SIZE + 1));
    std::cout << "Error = " << e << " " << e2 << " " << ", Time = " << timer() << "(ms)." << std::endl;

    std::fstream fp("res.dat", std::ios::out);
    for (Int i = 0; i <= GRID_SIZE; i++)
    {
        fp << (Real)i / (Real)GRID_SIZE << " ";
        fp << res.at(i) << std::endl;
    }
    fp.close();
    fp.open("u.dat", std::ios::out);
    for (Int i = 0; i <= GRID_SIZE; i++)
    {
        fp << (Real)i / (Real)GRID_SIZE << " ";
        fp << u((Real)i / (Real)GRID_SIZE) << std::endl;
    }
    fp.close();

    FILE *pipe = popen("gnuplot", "w");
    assert(pipe != nullptr);
    fprintf(pipe, "set terminal png size %d, %d\n", 640, 480);
    fprintf(pipe, "set output 'out.png'\n");
    fprintf(pipe, "unset key\n");
    // fprintf(pipe, "set xrange[0.0: 1.0]\n");
    // fprintf(pipe, "set yrange[0.0: 10.0]\n");
    fprintf(pipe, "plot \"res.dat\" using 1:2 with lines, \"u.dat\" using 1:2 with lines\n");
    fprintf(pipe, "unset output\n");
    pclose(pipe);

    return 0;
}