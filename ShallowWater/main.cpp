#include "SWE.h"

using namespace mw;

void SWE::init()
{
    for (Int i = 0; i < TOTAL_SIZE; i++)
    {
        // Real x = DX * i - DX / 2 - 5;
        // h.at(i) = 1 + 2 * std::exp(-5 * x * x) / 5;
        h.at(i) = (i < (TOTAL_SIZE / 3)) ? 7.0 : 3.0;
        u.at(i) = 0.0;
    }
    return;
}

void SWE::boundary()
{
    // h.front() = 80.0;
    // h.back() = 10.0;
    // u.front() = 0.0;
    // u.back() = 0.0;
    h.front() = h.at(X_MIN);
    h.back() = h.at(X_MAX);
    u.front() = -u.at(X_MIN);
    u.back() = -u.at(X_MAX);
}

int main()
{
    SWE swe;
    swe.save();
    for (Int i = 1; i <= 90 * DT_PER_SECOND; i++)
    {
        swe.step();
        if (i % (DT_PER_SECOND / 10) == 0)
        {
            std::cout << i << std::endl;
            swe.save();
            // getchar();
        }
    }
    return 0;
}