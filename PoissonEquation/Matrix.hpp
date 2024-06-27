#include <cassert>
#include <cstring>

#include "Matrix.h"

// #define MW_ASSERT

namespace mw
{
    template <typename Int, typename Real>
    Matrix<Int, Real> operator+(const Matrix<Int, Real> &m1, const Matrix<Int, Real> &m2)
    {
#ifdef MW_ASSERT
        assert(m1.height == m2.height);
        assert(m1.width == m2.width);
#endif
        Matrix res = m1;
        for (Int i = 0; i < res.width; i++)
        {
            for (Int j = 0; j < res.height; j++)
            {
                res(i, j) += m2(i, j);
            }
        }
        return res;
    }

    template <typename Int, typename Real>
    Matrix<Int, Real> operator-(const Matrix<Int, Real> &m1, const Matrix<Int, Real> &m2)
    {
#ifdef MW_ASSERT
        assert(m1.height == m2.height);
        assert(m1.width == m2.width);
#endif
        Matrix res = m1;
        for (Int i = 0; i < res.width; i++)
        {
            for (Int j = 0; j < res.height; j++)
            {
                res(i, j) -= m2(i, j);
            }
        }
        return res;
    }

    template <typename Int, typename Real>
    std::ostream &operator<<(std::ostream &out, const Matrix<Int, Real> &m)
    {
        for (Int j = 0; j < m.height; j++)
        {
            for (Int i = 0; i < m.width; i++)
            {
                out << m(i, j) << " ";
            }
            out << std::endl;
        }
        return out;
    }

    template <typename Int, typename Real>
    Matrix<Int, Real>::Matrix(const Int width, const Int height) : width(width), height(height)
    {
#ifdef MW_ASSERT
        assert(width > 0);
        assert(height > 0);
#endif
        data = new Real *[width];
        for (Int i = 0; i < width; i++)
        {
            data[i] = new Real[height];
            memset(data[i], 0, height * sizeof(Real));
        }
    }

    template <typename Int, typename Real>
    Matrix<Int, Real>::Matrix(const Matrix<Int, Real> &domain) : Matrix(domain.width, domain.height)
    {
        for (Int i = 0; i < width; i++)
        {
            memcpy(data[i], domain.data[i], height * sizeof(Real));
        }
    }

    template <typename Int, typename Real>
    Matrix<Int, Real>::~Matrix()
    {
        for (Int i = 0; i < width; i++)
        {
            delete[] data[i];
        }
        delete[] data;
    }

    template <typename Int, typename Real>
    const Real &Matrix<Int, Real>::operator()(const Int &x, const Int &y) const
    {
#ifdef MW_ASSERT
        assert(x >= 0 && x < width);
        assert(y >= 0 && y < height);
#endif
        return data[x][y];
    }

    template <typename Int, typename Real>
    Real &Matrix<Int, Real>::operator()(const Int &x, const Int &y)
    {
#ifdef MW_ASSERT
        assert(x >= 0 && x < width);
        assert(y >= 0 && y < height);
#endif
        return data[x][y];
    }

    template <typename Int, typename Real>
    Matrix<Int, Real> &Matrix<Int, Real>::operator=(const Matrix<Int, Real> &domain)
    {
        if (this != &domain)
        {
#ifdef MW_ASSERT
            assert(width == domain.width);
            assert(height == domain.height);
#endif
            for (Int i = 0; i < width; i++)
            {
                memcpy(data[i], domain.data[i], height * sizeof(Real));
            }
        }
        return (*this);
    }
};