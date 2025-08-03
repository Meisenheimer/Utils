#include <cmath>
#include <numbers>
#include <iostream>
#include <functional>

#include <Eigen/Eigen>

using Int = long long int;
using Real = long double;

constexpr Int GRID_SIZE = 512;

constexpr Real L = 1.0;
constexpr Real T = 30.0;
constexpr Real A = 1.0;

using Vector = Eigen::Vector<Real, GRID_SIZE>;

constexpr Int MIN = 0;
constexpr Int MAX = GRID_SIZE - 1;
constexpr Real DS = L / GRID_SIZE;
constexpr Real DT = DS / A;

constexpr Real eta(const Real &x, const Real &t);

const Vector F(const Vector &u, const Real &t);

const Vector FE(const Vector &u, const Real &t, const std::function<Vector(const Vector &, const Real &)> &F);
const Vector SSPRK3(const Vector &u, const Real &t, const std::function<Vector(const Vector &, const Real &)> &F);

template <typename Type>
const Vector test(const Type &IVP);

int main()
{
    Vector real;
    for (size_t i = MIN; i <= MAX; i++)
    {
        real(i) = eta(i * DS, T);
    }
    Vector res1 = test(FE);
    Vector res2 = test(SSPRK3);

    std::cout << (res1 - real).lpNorm<2>() << std::endl;
    std::cout << (res2 - real).lpNorm<2>() << std::endl;

    return 0;
}

inline constexpr Real eta(const Real &x, const Real &t)
{
    auto f = [](const Real &x)
    {
        return std::sin(2.0 * std::numbers::pi * x / L);
    };
    return f(x - A * t);
}

inline const Vector F(const Vector &u, const Real &t)
{
    Vector res;
    res(0) = (u(1) - u(MAX)) / (2.0 * DS);
    for (size_t i = (MIN + 1); i <= (MAX - 1); i++)
    {
        res(i) = (u(i + 1) - u(i - 1)) / (2.0 * DS);
    }
    res(MAX) = (u(0) - u(MAX - 1)) / (2.0 * DS);
    return res;
}

inline const Vector FE(const Vector &u, const Real &t, const std::function<Vector(const Vector &, const Real &)> &F)
{
    return u + DT * F(u, t);
}

inline const Vector SSPRK3(const Vector &u, const Real &t, const std::function<Vector(const Vector &, const Real &)> &F)
{
    Vector y1 = u + DT * F(u, t);
    Vector y2 = (3.0 * u + y1 + DT * F(y1, t + DT)) / 4.0;
    return (u + 2.0 * y2 + 2.0 * DT * F(y2, t + DT / 2.0)) / 3.0;
}

template <typename Type>
const Vector test(const Type &IVP)
{
    Real t = 0.0;
    Vector u;
    for (size_t i = MIN; i <= MAX; i++)
    {
        u(i) = eta(i * DS, t);
    }
    while (t < T)
    {
        u = IVP(u, t, F);
        t += DT;
    }
    return u;
}
