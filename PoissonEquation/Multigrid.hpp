#include <algorithm>
#include <iostream>
#include <cassert>

#include <Eigen/Eigen>

#include "Multigrid.h"

// #define MW_ASSERT

namespace mw
{
    template <typename Int, typename Real>
    Matrix<Int, Real> poissonSolver(const Matrix<Int, Real> &f,
                                    const Matrix<Int, Real> &alpha, const Matrix<Int, Real> &beta,
                                    const Real &dx, const Real &dy)
    {
        Eigen::SparseMatrix<Real> A(f.width * f.height, f.width * f.height);
        Eigen::Vector<Real, Eigen::Dynamic> b(f.width * f.height);
        for (Int x = 0, k = 0; x < f.width; x++)
        {
            for (Int y = 0; y < f.height; y++, k++)
            {
                if (x == 0)
                {
                    A.insert(k, x + y * f.width) = alpha(x, y) + beta(x, y) / dx;
                    A.insert(k, x + 1 + y * f.width) = -beta(x, y) / dx;
                }
                else if (x == (f.width - 1))
                {
                    A.insert(k, x + y * f.width) = alpha(x, y) + beta(x, y) / dx;
                    A.insert(k, x - 1 + y * f.width) = -beta(x, y) / dx;
                }
                else if (y == 0)
                {
                    A.insert(k, x + y * f.width) = alpha(x, y) + beta(x, y) / dy;
                    A.insert(k, x + (y + 1) * f.width) = -beta(x, y) / dy;
                }
                else if (y == (f.width - 1))
                {
                    A.insert(k, x + y * f.width) = alpha(x, y) + beta(x, y) / dy;
                    A.insert(k, x + (y - 1) * f.width) = -beta(x, y) / dy;
                }
                else
                {
                    A.insert(k, x + y * f.width) = -2.0 / (dx * dx) - 2.0 / (dy * dy);
                    A.insert(k, (x + 1) + y * f.width) = 1.0 / (dx * dx);
                    A.insert(k, (x - 1) + y * f.width) = 1.0 / (dx * dx);
                    A.insert(k, x + (y + 1) * f.width) = 1.0 / (dy * dy);
                    A.insert(k, x + (y - 1) * f.width) = 1.0 / (dy * dy);
                }
                b(k) = f(x, y);
            }
        }
        Eigen::SparseLU<Eigen::SparseMatrix<Real>, Eigen::COLAMDOrdering<int>> solver;
        A.makeCompressed();
        solver.compute(A);
        Eigen::Vector<Real, Eigen::Dynamic> r = solver.solve(b);
        Matrix<Int, Real> res(f.width, f.height);
        for (Int x = 0, k = 0; x < f.width; x++)
        {
            for (Int y = 0; y < f.height; y++, k++)
            {
                res(x, y) = r(k);
            }
        }
        return res;
    }

    template <typename Int, typename Real>
    inline Matrix<Int, Real> Poisson<Int, Real>::restriction(const Matrix<Int, Real> &u)
    {
        Int n = (u.width - 1) / 2;
        Matrix<Int, Real> res(n + 1, n + 1);
        for (Int i = 0; i <= n; i++)
        {
            for (Int j = 0; j <= n; j++)
            {
                res(i, j) = u(2 * i, 2 * j);
            }
        }
        return res;
    }

    template <typename Int, typename Real>
    inline Matrix<Int, Real> Poisson<Int, Real>::prolongation(const Matrix<Int, Real> &u)
    {
        Int n = u.width - 1, i, j;
        Matrix<Int, Real> res(2 * n + 1, 2 * n + 1);
        for (i = 0; i <= n; i++)
        {
            for (j = 0; j < n; j += 2)
            {
                res(2 * i, 2 * j) = u(i, j);
                res(2 * i, 2 * j + 2) = u(i, j + 1);
                res(2 * i, 2 * j + 4) = u(i, j + 2);
                res(2 * i, 2 * j + 1) = (3.0 * u(i, j) + 6.0 * u(i, j + 1) - u(i, j + 2)) / 8.0;
                res(2 * i, 2 * j + 3) = (-u(i, j) + 6.0 * u(i, j + 1) + 3.0 * u(i, j + 2)) / 8.0;
            }
            res(2 * i, 2 * n) = u(i, n);
        }
        n = 2 * n;
        for (j = 0; j <= n; j++)
        {
            for (i = 0; i < n; i += 4)
            {
                res(i + 1, j) = (3.0 * res(i, j) + 6.0 * res(i + 2, j) - res(i + 4, j)) / 8.0;
                res(i + 3, j) = (-res(i, j) + 6.0 * res(i + 2, j) + 3.0 * res(i + 4, j)) / 8.0;
            }
        }
        return res;
    }

    template <typename Int, typename Real>
    inline Matrix<Int, Real> Poisson<Int, Real>::iterate(const Matrix<Int, Real> &m, const Matrix<Int, Real> &rhs)
    {
#ifdef MW_ASSERT
        assert(m.width == m.height);
#endif
        constexpr Real w = 0.8;
        const Int gridsize = m.width - 1;
        const Real dx = (pos[RIGHT] - pos[LEFT]) / gridsize;
        const Real dy = (pos[TOP] - pos[BOTTOM]) / gridsize;
        const Real dx2 = dx * dx;
        const Real dy2 = dy * dy;
        const Real k = (2.0 / dx2 + 2.0 / dy2);

        Real x, y;
        Matrix<Int, Real> v = Matrix<Int, Real>(gridsize + 1, gridsize + 1);

        // Boundary
        for (Int i = 0; i <= gridsize; i++)
        {
            Real x = getX(i, gridsize);
            Real y = getY(0, gridsize);
            v(i, 0) = (1 - w) * m(i, 0) + w * (beta(x, y) * m(i, 1) / dy + rhs(i, 0)) / (alpha(x, y) + beta(x, y) / dy);
            y = getY(gridsize, gridsize);
            v(i, gridsize) = (1 - w) * m(i, gridsize) + w * (beta(x, y) * m(i, gridsize - 1) / dy + rhs(i, gridsize)) / (alpha(x, y) + beta(x, y) / dy);
        }
        for (Int j = 1; j < gridsize; j++)
        {
            Real y = getY(j, gridsize);
            Real x = getX(0, gridsize);
            v(0, j) = (1 - w) * m(0, j) + w * (beta(x, y) * m(1, j) / dx + rhs(0, j)) / (alpha(x, y) + beta(x, y) / dx);
            x = getX(gridsize, gridsize);
            v(gridsize, j) = (1 - w) * m(gridsize, j) + w * (beta(x, y) * m(gridsize - 1, j) / dx + rhs(gridsize, j)) / (alpha(x, y) + beta(x, y) / dx);
        }

        // Inner
        for (Int i = 1; i < gridsize; i++)
        {
            for (Int j = 1; j < gridsize; j++)
            {
                v(i, j) = (1 - w) * m(i, j) + w * ((m(i + 1, j) + m(i - 1, j)) / dx2 + (m(i, j - 1) + m(i, j + 1)) / dy2 - rhs(i, j)) / k;
            }
        }

        return v;
    }

    template <typename Int, typename Real>
    inline Matrix<Int, Real> Poisson<Int, Real>::VC(Matrix<Int, Real> v, const Matrix<Int, Real> &rhs)
    {
        Int gridsize = v.width - 1;
        for (Int i = 0; i < REPEAT_TIME_1; i++)
        {
            v = iterate(v, rhs);
        }
        if (gridsize > MIN_SIZE)
        {
            v = v + prolongation(VC(Matrix<Int, Real>(gridsize / 2 + 1, gridsize / 2 + 1), restriction(residual(v, rhs))));
        }
        else
        {
            for (Int i = 0; i < (REPEAT_TIME_1 + REPEAT_TIME_2); i++)
            {
                v = iterate(v, rhs);
            }
        }
        for (Int i = 0; i < REPEAT_TIME_2; i++)
        {
            v = iterate(v, rhs);
        }
        return v;
    }

    template <typename Int, typename Real>
    inline Matrix<Int, Real> Poisson<Int, Real>::FMG(const Matrix<Int, Real> &rhs)
    {
        Int gridsize = rhs.width - 1;
        Matrix<Int, Real> v(gridsize + 1, gridsize + 1);
        if (gridsize > MIN_SIZE)
        {
            v = prolongation(FMG(restriction(rhs)));
        }
        v = VC(v, rhs);
        return v;
    }

    template <typename Int, typename Real>
    inline Matrix<Int, Real> Poisson<Int, Real>::residual(const Matrix<Int, Real> &v, const Matrix<Int, Real> &rhs)
    {
#ifdef MW_ASSERT
        assert(v.width == v.height);
#endif
        const Int gridsize = v.width - 1;
        const Real dx = (pos[RIGHT] - pos[LEFT]) / gridsize;
        const Real dy = (pos[TOP] - pos[BOTTOM]) / gridsize;
        const Real dx2 = dx * dx;
        const Real dy2 = dy * dy;
        const Real k = (2.0 / dx2 + 2.0 / dy2);

        Real x, y;
        Matrix<Int, Real> res = rhs;

        // Boundary
        for (Int i = 0; i <= gridsize; i++)
        {
            Real x = getX(i, gridsize);
            Real y = getY(0, gridsize);
            res(i, 0) -= ((alpha(x, y) + beta(x, y) / dy) * v(i, 0) - beta(x, y) * v(i, 1) / dy);
            y = getY(gridsize, gridsize);
            res(i, gridsize) -= ((alpha(x, y) + beta(x, y) / dy) * v(i, gridsize) - beta(x, y) * v(i, gridsize - 1) / dy);
        }
        for (Int j = 1; j < gridsize; j++)
        {
            Real y = getY(j, gridsize);
            Real x = getX(0, gridsize);
            res(0, j) -= ((alpha(x, y) + beta(x, y) / dx) * v(0, j) - beta(x, y) * v(1, j) / dx);
            x = getX(gridsize, gridsize);
            res(gridsize, j) -= ((alpha(x, y) + beta(x, y) / dx) * v(gridsize, j) - beta(x, y) * v(gridsize - 1, j) / dx);
        }
        // Inner
        for (Int i = 1; i < gridsize; i++)
        {
            for (Int j = 1; j < gridsize; j++)
            {
                res(i, j) -= (-k * v(i, j) + (v(i + 1, j) + v(i - 1, j)) / dx2 + (v(i, j - 1) + v(i, j + 1)) / dy2);
            }
        }

        return res;
    }

    template <typename Int, typename Real>
    inline Poisson<Int, Real>::Poisson(const Int &grid_size, const std::vector<Real> &pos)
        : grid_size(grid_size), pos(pos), u(grid_size + 1, grid_size + 1)
    {
        Int tmp = grid_size;
        while (tmp > 1)
        {
            assert((tmp % 2 == 0));
            tmp /= 2;
        }
    }

    template <typename Int, typename Real>
    inline void Poisson<Int, Real>::setBoundary(const Poisson<Int, Real>::Func &f, const Poisson<Int, Real>::Func &g, const Poisson<Int, Real>::Func &alpha, const Poisson<Int, Real>::Func &beta)
    {
        this->f = f;
        this->g = g;
        this->alpha = alpha;
        this->beta = beta;
        return;
    }

    template <typename Int, typename Real>
    inline void Poisson<Int, Real>::solve(const Int &n1, const Int &n2)
    {
        Real x, y;
        Matrix<Int, Real> rhs(grid_size + 1, grid_size + 1);
        for (Int i = 0; i <= grid_size; i++)
        {
            x = getX(i, grid_size);
            for (Int j = 0; j <= grid_size; j++)
            {
                if (i == 0 || j == 0 || i == grid_size || j == grid_size)
                {
                    rhs(i, j) = g(x, getY(j, grid_size));
                }
                else
                {
                    rhs(i, j) = f(x, getY(j, grid_size));
                }
            }
        }
        this->u = Matrix<Int, Real>(grid_size + 1, grid_size + 1);
        this->u = FMG(rhs);
        for (Int i = 0; i < n1; i++)
        {
            this->u = VC(this->u, rhs);
        }
        for (Int i = 0; i < n2; i++)
        {
            this->u = iterate(this->u, rhs);
        }
        return;
    }

    template <typename Int, typename Real>
    inline const Matrix<Int, Real> &Poisson<Int, Real>::result() const
    {
        return u;
    }

    template <typename Int, typename Real>
    inline Real Poisson<Int, Real>::getX(const Int &k, const Int &size)
    {
#ifdef MW_ASSERT
        assert(k >= 0 && k <= size);
#endif
        return (k * (pos[RIGHT] - pos[LEFT]) / size) + pos[LEFT];
    }

    template <typename Int, typename Real>
    inline Real Poisson<Int, Real>::getY(const Int &k, const Int &size)
    {
#ifdef MW_ASSERT
        assert(k >= 0 && k <= size);
#endif
        return (k * (pos[TOP] - pos[BOTTOM]) / size) + pos[BOTTOM];
    }
};