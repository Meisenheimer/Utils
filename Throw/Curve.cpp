#include <iostream>

#include "Curve.h"

Real distance(const Point &p1, const Point &p2)
{
    return (p1 - p2).lpNorm<2>();
}

Real distance(const Point &p, const Line &l)
{
    const Vector u = p - l.p1;
    const Vector v = l.p2 - l.p1;
    const Point q = l.p1 + (u.dot(v)) * u;
    if (l(q))
    {
        return distance(p, q);
    }
    return std::min(distance(p, l.p1), distance(p, l.p2));
}

Real distance(const Line &l, const Point &p)
{
    return distance(p, l);
}

Real cross(const Vector &v1, const Vector &v2)
{
    return v1(0) * v2(1) - v1(1) * v2(0);
}

Point makePoint(const Real &x, const Real &y)
{
    Point p;
    p(0) = x;
    p(1) = y;
    return p;
}

Line::Line(const Point &p1, const Point &p2)
{
    this->p1 = p1;
    this->p2 = p2;
    this->k = ((p2(1) - p1(1)) / (p2(0) - p1(0)));
    this->d = p1(1) - k * p1(0);
}

Bool Line::intersect(const Real &a, const Real &b, const Real &c) const
{
    Real na = a, nb = b - k, nc = c - d;
    Real delta = (nb * nb - 4 * na * nc);
    Real x;
    if (delta >= 0.0)
    {
        delta = std::sqrt(delta);
        if (std::abs(na) <= DELTA)
        {
            na = DELTA;
        }
        x = (-nb + delta) / (2 * na);
        if (this->operator()(makePoint(x, k * x + d)))
        {
            return true;
        }
        x = (-nb - delta) / (2 * na);
        if (this->operator()(makePoint(x, k * x + d)))
        {
            return true;
        }
    }
    return false;
}

Bool Line::inside(const Point &c, const Real &r) const
{
    return distance(*this, c) <= r;
}

Bool Line::operator()(const Point &p) const
{
    if (distance(p, p1) < DELTA || distance(p, p1) < DELTA)
    {
        return false;
    }
    const Vector v = (p - p1);
    const Vector u = (p2 - p1);
    const Real k1 = v(0) / u(0);
    const Real k2 = v(1) / u(1);
    return (k1 > -DELTA && k2 > -DELTA && k1 < (1 + DELTA) && k2 < (1 + DELTA) && std::abs(k1 - k2) < DELTA);
}

Curve::Curve(const Point &p1, const Point &p2, const Point &p3)
{
    Eigen::Matrix<Real, 3, 3> A;
    Eigen::Vector<Real, 3> y;
    y(0) = p1(1);
    y(1) = p2(1);
    y(2) = p3(1);
    A(0, 0) = p1(0) * p1(0);
    A(0, 1) = p1(0);
    A(1, 0) = p2(0) * p2(0);
    A(1, 1) = p2(0);
    A(2, 0) = p3(0) * p3(0);
    A(2, 1) = p3(0);
    A(0, 2) = A(1, 2) = A(2, 2) = 1.0;
    Eigen::Vector<Real, 3> res = A.fullPivHouseholderQr().solve(y);
    this->a = res(0);
    this->b = res(1);
    this->c = res(2);
}

Curve::Curve(const Real &a, const Point &p1, const Point &p2)
{
    const Real x1 = p1(0);
    const Real y1 = p1(1);
    const Real x2 = p2(0);
    const Real y2 = p2(1);
    const Real k = (y2 - y1) / (x2 - x1);
    this->a = a;
    this->b = k - a * (x1 + x2);
    this->c = y1 - k * x1 + a * x1 * x2;
    return;
}

Real Curve::operator()(const Real &x) const
{
    return a * x * x + b * x + c;
}