#define EIGEN_NO_DEBUG
#define NDEBUG

#include <iostream>
#include <cmath>
#include <limits>
#include <cstddef>
#include <chrono>

#include <autodiff/reverse/var.hpp>
#include <autodiff/reverse/var/eigen.hpp>

using Int = long long int;
using Real = long double;
using autodiff::Variable;
template <typename Type>
using Matrix = Eigen::Matrix<Type, Eigen::Dynamic, Eigen::Dynamic>;
template <typename Type>
using Vector = Eigen::Vector<Type, Eigen::Dynamic>;

constexpr Int LOG_INTERVAL = 100;
constexpr Int REPEAT_TIME = 5000;
constexpr Int GRID_SIZE = 12;
constexpr Real SIZE = 1.0;
constexpr Real H = SIZE / GRID_SIZE;
constexpr Real HEIGHT = 1.0;

Matrix<Real> u(GRID_SIZE + 1, GRID_SIZE + 1);

inline size_t timer()
{
    static std::chrono::_V2::steady_clock::time_point time = std::chrono::steady_clock::now();
    std::chrono::_V2::steady_clock::time_point now = std::chrono::steady_clock::now();
    size_t duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - time).count();
    time = now;
    return duration;
}

inline void outputImage(const Int &n)
{
    // Output results to png file.
    FILE *pipe = popen("gnuplot", "w");
    fprintf(pipe, "set terminal png size %d, %d\n", 2 * 640, 2 * 480);
    fprintf(pipe, "set output 'res/res_%lld.png'\n", n);
    fprintf(pipe, "unset title\n");
    fprintf(pipe, "unset key\n");
    fprintf(pipe, "set xrange[-0.4: %lld.4]\n", GRID_SIZE);
    fprintf(pipe, "set yrange[-0.4: %lld.4]\n", GRID_SIZE);
    fprintf(pipe, "set grid\n");
    fprintf(pipe, "set hidden3d\n");
    fprintf(pipe, "$grid << EOD\n");
    for (Int j = 0; j <= GRID_SIZE; j++)
    {
        for (Int i = 0; i <= GRID_SIZE; i++)
        {
            Variable<Real> tmp = u(i, j);
            fprintf(pipe, "%Lf ", (Real)tmp);
        }
        fprintf(pipe, "\n");
    }
    fprintf(pipe, "EOD\n");
    fprintf(pipe, "splot '$grid' matrix with lines notitle\n");
    fprintf(pipe, "unset output\n");
    pclose(pipe);
    return;
};

inline Vector<Real> df(const Int &i, const Real &x, const Real &y)
{
    Vector<Real> res = Vector<Real>::Zero(2);
    switch (i)
    {
    case 1:
        res(0) = y - H;
        res(1) = x - H;
        break;
    case 2:
        res(0) = H - y;
        res(1) = -x;
        break;
    case 3:
        res(0) = -y;
        res(1) = H - x;
        break;
    case 4:
        res(0) = y;
        res(1) = x;
        break;
    default:
        printf("Error.");
        exit(0);
    }
    return res / (H * H);
}

inline constexpr Real step(const Int &n)
{
    constexpr Real STEP = 1e-2;
    constexpr Real TARGET = 1e-3;
    constexpr Int CHANGE_POINT = REPEAT_TIME / 20;
    return n <= CHANGE_POINT ? STEP : STEP + (TARGET - STEP) * (n - CHANGE_POINT) / (REPEAT_TIME - CHANGE_POINT);
}

int main()
{
    // Initial value and boundary condition.
    for (Int i = 0; i <= GRID_SIZE; i++)
    {
        for (Int j = 0; j <= GRID_SIZE; j++)
        {
            u(i, j) = HEIGHT * (Real)i / (Real)GRID_SIZE;
        }
    }
    Matrix<Variable<Real>> tmp[4];
    tmp[0].resize(GRID_SIZE, GRID_SIZE);
    tmp[1].resize(GRID_SIZE, GRID_SIZE);
    tmp[2].resize(GRID_SIZE, GRID_SIZE);
    tmp[3].resize(GRID_SIZE, GRID_SIZE);
    timer();
    for (Int l = 1; l <= REPEAT_TIME; l++)
    {
        // Clear gradient.
        Matrix<Variable<Real>> v = u;
        Matrix<Real> grad = Matrix<Real>::Zero(GRID_SIZE + 1, GRID_SIZE + 1);
#pragma omp parallel for
        for (Int i = 0; i < GRID_SIZE; i++)
        {
            for (Int j = 0; j < GRID_SIZE; j++)
            {
                for (Int k = 1; k <= 4; k++)
                {
                    std::function<Variable<Real>(const Real &, const Real &)> F =
                        [&](const Real &x, const Real &y) -> Variable<Real>
                    {
                        Vector<Variable<Real>> s = v(i, j) * df(1, x, y) +
                                                   v(i + 1, j) * df(2, x, y) +
                                                   v(i, j + 1) * df(3, x, y) +
                                                   v(i + 1, j + 1) * df(4, x, y);
                        return s.dot(df(k, x, y)) / sqrt(s.lpNorm<2>() + 1.0);
                    };
                    tmp[k - 1](i, j) = (H * H / 12.0) * (F(0, 0) + F(H, 0) + F(0, H) + F(H, H) + 8 * F(H / 2, H / 2) +
                                                         4 * F(0, H / 2) + 4 * F(H, H / 2) + 4 * F(H / 2, 0) + 4 * F(H / 2, H));
                    tmp[k - 1](i, j) = tmp[k - 1](i, j) * tmp[k - 1](i, j);
                }
            }
        }
        constexpr Int OMP_N = 2;
        for (Int omp_n = 0; omp_n < OMP_N; omp_n++)
        {
#pragma omp parallel for
            for (Int i = omp_n; i < GRID_SIZE; i += OMP_N)
            {
                for (Int j = 0; j < GRID_SIZE; j++)
                {
                    for (Int k = 0; k < 4; k++)
                    {
                        grad(i, j) += Real(derivatives(tmp[k](i, j), wrt(v(i, j)))[0]);
                        grad(i + 1, j) += Real(derivatives(tmp[k](i, j), wrt(v(i + 1, j)))[0]);
                        grad(i, j + 1) += Real(derivatives(tmp[k](i, j), wrt(v(i, j + 1)))[0]);
                        grad(i + 1, j + 1) += Real(derivatives(tmp[k](i, j), wrt(v(i + 1, j + 1)))[0]);
                    }
                }
            }
        }
        Real tmp_step = step(l), gm = 0.0, gms = 0.0, gmax = 0.0;
        for (Int i = 0; i < GRID_SIZE; i++)
        {
            for (Int j = 1; j < GRID_SIZE; j++)
            {
                Real g = grad(i, j);
                u(i, j) -= g * tmp_step;
                g = std::abs(g);
                gm += g;
                gms += g * g;
                if (g > gmax)
                {
                    gmax = g;
                }
            }
        }
        gm /= (GRID_SIZE - 1) * GRID_SIZE;
        gms /= (GRID_SIZE - 1) * GRID_SIZE;
        // Output log.
        if (l % LOG_INTERVAL == 0)
        {
            outputImage(l);
            printf("Iter %lld: g = (%Le, %Le, %Le), step = %Le, time = %llu\n\n", l, gm, gms, gmax, tmp_step, timer());
        }
    }
    // Check the derivative of free boundary.
    for (Int i = 0; i <= GRID_SIZE; i++)
    {
        Variable<Real> tmp = u(0, i) - u(1, i);
        printf("%lld: %Le\n", i, (Real)tmp / H);
    }
    return 0;
}
