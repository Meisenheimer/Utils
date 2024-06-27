#ifndef MW_MULTIGRID_H
#define MW_MULTIGRID_H

#include <cmath>
#include <tuple>
#include <functional>

#include "Matrix.h"

namespace mw
{

    template <typename Int, typename Real>
    class Poisson;

    template <typename Int, typename Real>
    Matrix<Int, Real> poissonSolver(const Matrix<Int, Real> &f,
                                    const Matrix<Int, Real> &alpha, const Matrix<Int, Real> &beta,
                                    const Real &dx, const Real &dy);

    template <typename Int, typename Real>
    class Poisson
    {
        static_assert(std::is_integral_v<Int>);
        static_assert(std::is_floating_point_v<Real>);

    private:
        using Func = std::function<Real(const Real &, const Real &)>;

        static Matrix<Int, Real> restriction(const Matrix<Int, Real> &m);
        static Matrix<Int, Real> prolongation(const Matrix<Int, Real> &m);

        inline constexpr static Int MIN_SIZE = 4;
        inline constexpr static Int REPEAT_TIME_1 = 3;
        inline constexpr static Int REPEAT_TIME_2 = 3;

        inline constexpr static Int LEFT = 0;
        inline constexpr static Int BOTTOM = 1;
        inline constexpr static Int RIGHT = 2;
        inline constexpr static Int TOP = 3;

        inline constexpr static Real DELTA = std::numeric_limits<float>::epsilon();

        static_assert(MIN_SIZE >= 4);
        static_assert(REPEAT_TIME_1 >= 1);
        static_assert(REPEAT_TIME_2 >= 1);

    private:
        const Int grid_size;

        const std::vector<Real> pos;

        Matrix<Int, Real> u;

        Func f;
        Func g;
        Func alpha;
        Func beta;

    public:
        Matrix<Int, Real> iterate(const Matrix<Int, Real> &m, const Matrix<Int, Real> &rhs);
        Matrix<Int, Real> VC(Matrix<Int, Real> v, const Matrix<Int, Real> &rhs);
        Matrix<Int, Real> FMG(const Matrix<Int, Real> &rhs);
        Matrix<Int, Real> residual(const Matrix<Int, Real> &v, const Matrix<Int, Real> &rhs);

    public:
        Poisson(const Int &grid_size, const std::vector<Real> &pos = {0.0, 0.0, 1.0, 1.0});

        void setBoundary(const Func &f, const Func &g, const Func &alpha, const Func &beta);

        void solve(const Int &n1 = 0, const Int &n2 = 0);
        const Matrix<Int, Real> &result() const;

        Real getX(const Int &k, const Int &size);
        Real getY(const Int &k, const Int &size);
    };
};

#include "Multigrid.hpp"

#endif
