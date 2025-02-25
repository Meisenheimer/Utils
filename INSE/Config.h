#ifndef MW_CONFIG_H
#define MW_CONFIG_H

#include <Eigen/Eigen>

#define MW_ALERT printf("%s %d\n", __FILE__, __LINE__);

namespace mw
{
    using Int = long long int;
    using Real = long double;

    using Matrix = Eigen::Matrix<Real, Eigen::Dynamic, Eigen::Dynamic>;
    using SparseMatrix = Eigen::SparseMatrix<Real>;
    using Vector = Eigen::Vector<Real, Eigen::Dynamic>;

    constexpr bool ARTIFICIAL_VISCOSITY = true;

    constexpr Int GRID_SIZE = 64 + 1;
    constexpr Int MIN = 0;
    constexpr Int MAX = GRID_SIZE - 1;
    constexpr Int STEP = (MAX - MIN) / 16;

    constexpr Real SIZE = 2.0;

    constexpr Real DS = SIZE / (GRID_SIZE - 1);
    constexpr Real DT = DS / 128.0;

    constexpr Real RHO = 1.0;
    constexpr Real NU = 1.0;

    constexpr Real G = 0.0;
    constexpr Real MU = 0.000001;

    constexpr Int ITERATION_CFD = 1 << 9;
    constexpr Int ITERATION_CDE = 16 / DT;
    constexpr Int RELAX_TIME = 1 << 4;

    inline const Matrix c1x(const Matrix &f);
    inline const Matrix c1y(const Matrix &f);
    inline const Matrix lapx(const Matrix &f);
    inline const Matrix lapy(const Matrix &f);
    inline const Matrix lap(const Matrix &f);
    inline const Matrix udux(const Matrix &u);
    inline const Matrix uduy(const Matrix &u);
    inline const Matrix adux(const Matrix &u, const Matrix &a);
    inline const Matrix aduy(const Matrix &u, const Matrix &a);

    inline const Matrix c1x(const Matrix &f)
    {
        Matrix res = Matrix::Zero(GRID_SIZE, GRID_SIZE);
        for (Int i = MIN + 1; i < MAX; i++)
        {
            for (Int j = MIN + 1; j < MAX; j++)
            {
                res(i, j) = (f(i + 1, j) - f(i - 1, j)) / (2.0 * DS);
            }
        }
        return res;
    }

    inline const Matrix c1y(const Matrix &f)
    {
        Matrix res = Matrix::Zero(GRID_SIZE, GRID_SIZE);
        for (Int i = MIN + 1; i < MAX; i++)
        {
            for (Int j = MIN + 1; j < MAX; j++)
            {
                res(i, j) = (f(i, j + 1) - f(i, j - 1)) / (2.0 * DS);
            }
        }
        return res;
    }

    inline const Matrix lapx(const Matrix &f)
    {
        Matrix res = Matrix::Zero(GRID_SIZE, GRID_SIZE);
        for (Int i = MIN + 1; i < MAX; i++)
        {
            for (Int j = MIN + 1; j < MAX; j++)
            {
                res(i, j) = (f(i + 1, j) + f(i - 1, j) - 2.0 * f(i, j)) / (DS * DS);
            }
        }
        return res;
    }

    inline const Matrix lapy(const Matrix &f)
    {
        Matrix res = Matrix::Zero(GRID_SIZE, GRID_SIZE);
        for (Int i = MIN + 1; i < MAX; i++)
        {
            for (Int j = MIN + 1; j < MAX; j++)
            {
                res(i, j) = (f(i, j + 1) + f(i, j - 1) - 2.0 * f(i, j)) / (DS * DS);
            }
        }
        return res;
    }

    inline const Matrix lap(const Matrix &f)
    {
        Matrix res = Matrix::Zero(GRID_SIZE, GRID_SIZE);
        for (Int i = MIN + 1; i < MAX; i++)
        {
            for (Int j = MIN + 1; j < MAX; j++)
            {
                res(i, j) = (f(i + 1, j) + f(i - 1, j) + f(i, j + 1) + f(i, j - 1) - 4.0 * f(i, j)) / (DS * DS);
            }
        }
        return res;
    }

    inline const Matrix udux(const Matrix &u)
    {
        if constexpr (ARTIFICIAL_VISCOSITY)
        {
            Matrix res = Matrix::Zero(GRID_SIZE, GRID_SIZE);
            for (Int i = MIN + 1; i < MAX; i++)
            {
                for (Int j = MIN + 1; j < MAX; j++)
                {
                    Real ur = (u(i, j) + u(i + 1, j)) / 2.0 -
                              DT * (u(i + 1, j) * u(i + 1, j) - u(i, j) * u(i, j)) / (2.0 * DS);
                    Real ul = (u(i - 1, j) + u(i, j)) / 2.0 -
                              DT * (u(i, j) * u(i, j) - u(i - 1, j) * u(i - 1, j)) / (2.0 * DS);
                    res(i, j) = (ur * ur - ul * ul) / (2.0 * DS);
                }
            }
            return res;
        }
        else
        {
            return u.cwiseProduct(c1x(u));
        }
    }

    inline const Matrix uduy(const Matrix &u)
    {
        if constexpr (ARTIFICIAL_VISCOSITY)
        {
            Matrix res = Matrix::Zero(GRID_SIZE, GRID_SIZE);
            for (Int i = MIN + 1; i < MAX; i++)
            {
                for (Int j = MIN + 1; j < MAX; j++)
                {
                    Real ut = (u(i, j) + u(i, j + 1)) / 2.0 -
                              DT * (u(i, j + 1) * u(i, j + 1) - u(i, j) * u(i, j)) / (2.0 * DS);
                    Real ub = (u(i, j - 1) + u(i, j)) / 2.0 -
                              DT * (u(i, j) * u(i, j) - u(i, j - 1) * u(i, j - 1)) / (2.0 * DS);
                    res(i, j) = (ut * ut - ub * ub) / (2.0 * DS);
                }
            }
            return res;
        }
        else
        {
            return u.cwiseProduct(c1y(u));
        }
    }

    inline const Matrix adux(const Matrix &u, const Matrix &a)
    {
        if constexpr (ARTIFICIAL_VISCOSITY)
        {
            return a.cwiseProduct(c1x(u)) - DT * a.cwiseProduct(a).cwiseProduct(lapx(u)) / 2.0;
        }
        else
        {
            return a.cwiseProduct(c1x(u));
        }
    }

    inline const Matrix aduy(const Matrix &u, const Matrix &a)
    {
        if constexpr (ARTIFICIAL_VISCOSITY)
        {
            return a.cwiseProduct(c1y(u)) - DT * a.cwiseProduct(a).cwiseProduct(lapy(u)) / 2.0;
        }
        else
        {
            return a.cwiseProduct(c1y(u));
        }
    }
};

#endif