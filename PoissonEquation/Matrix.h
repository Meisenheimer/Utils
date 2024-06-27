#ifndef MW_MATRIX_H
#define MW_MATRIX_H

#include <ostream>

namespace mw
{
    template <typename Int, typename Real>
    class Matrix;

    template <typename Int, typename Real>
    Matrix<Int, Real> operator+(const Matrix<Int, Real> &m1, const Matrix<Int, Real> &m2);
    template <typename Int, typename Real>
    Matrix<Int, Real> operator-(const Matrix<Int, Real> &m1, const Matrix<Int, Real> &m2);
    template <typename Int, typename Real>
    std::ostream &operator<<(std::ostream &out, const Matrix<Int, Real> &m);

    template <typename Int, typename Real>
    class Matrix
    {
    private:
        Real **data;

    public:
        const Int width;
        const Int height;

    public:
        Matrix(const Int width, const Int height);
        Matrix(const Matrix &domain);
        ~Matrix();

        const Real &operator()(const Int &x, const Int &y) const;
        Real &operator()(const Int &x, const Int &y);

        Matrix &operator=(const Matrix &domain);
    };
};

#include "Matrix.hpp"

#endif