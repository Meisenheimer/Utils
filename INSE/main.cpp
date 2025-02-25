// #define EIGEN_NO_DEBUG
// #define NDEBUG

#include <chrono>

#include "CFD.h"
#include "CDE.h"

using namespace mw;

inline size_t timer()
{
    static std::chrono::_V2::steady_clock::time_point time = std::chrono::steady_clock::now();
    std::chrono::_V2::steady_clock::time_point now = std::chrono::steady_clock::now();
    size_t duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - time).count();
    time = now;
    return duration;
}

inline void CFD::init()
{
    v.fill(0.0);
    u.fill(0.0);
    p.fill(0.0);
    fx.fill(0.0);
    fy.fill(G);
    boundary_uv();
    return;
}

inline void CFD::boundary_uv()
{
    for (Int i = MIN; i <= MAX; i++)
    {
        v(i, MIN) = 0.0;
        v(i, MAX) = 0.0;

        u(i, MIN) = 0.0;
        u(i, MAX) = 5.0;
        // u(i, MAX) = std::min(1.0L, t * DT);

        v(MIN, i) = 0.0;
        v(MAX, i) = 0.0;

        u(MIN, i) = 0.0;
        u(MAX, i) = 0.0;
    }
    return;
}

inline void CFD::boundary_p()
{
    for (Int i = MIN; i <= MAX; i++)
    {
        p(i, MAX) = p(i, MAX - 1);
        p(MIN, i) = p(MIN + 1, i);
        p(i, MIN) = p(i, MIN + 1);
        p(MAX, i) = p(MAX - 1, i);
        // p(MAX, i) = 0;
    }
    return;
}

inline void CDE::init()
{
    f.fill(0.0);
    r.fill(0.0);
    boundary();
    return;
}

inline void CDE::boundary()
{
    r(MAX / 2, MAX / 2) = 0.0;
    if (t >= (3 * ITERATION_CDE / 4))
    {
        r(MAX / 2, MAX / 2) = 0.0;
    }
    else if ((t % 2048) <= 1024)
    {
        r(MAX / 2, MAX / 2) = 10.0 / (DS * DS);
    }
    return;
}

int main()
{
    timer();

    mw::CFD cfd;
    cfd.save();
    for (Int i = 1; i <= ITERATION_CFD; i++)
    {
        cfd.step();
        if (i % (ITERATION_CFD / 128) == 0)
        {
            if (cfd.check())
            {
                cfd.print();
                exit(0);
            }
            printf("Progress: %6.2lf%%\r", (100 * (double)i / ITERATION_CFD));
        }
    }
    printf("Progress: 100.00%%\n");
    cfd.save();
    auto r = cfd.result();
    Matrix u = r.first;
    Matrix v = r.second;

    mw::CDE cde(u, v);
    for (Int i = 1; i <= ITERATION_CDE; i++)
    {
        cde.step();
        if (i % (ITERATION_CDE / 256) == 0)
        {
            cde.save();
            if (cde.check())
            {
                cde.print();
                exit(0);
            }
            printf("Progress: %6.2lf%%\r", (100 * (double)i / ITERATION_CDE));
        }
    }
    printf("Time: %6lld (ms)\n", (long long int)timer());

    return 0;
}