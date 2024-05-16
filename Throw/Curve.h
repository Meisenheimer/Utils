#ifndef CURVE_H
#define CURVE_H

#include "Config.h"

struct Line;
struct Curve;

Real distance(const Point &p1, const Point &p2);
Real distance(const Point &p, const Line &l);
Real distance(const Line &l, const Point &p);
Point makePoint(const Real &x, const Real &y);

struct Line
{
    Point p1;
    Point p2;
    Real k;
    Real d;

public:
    Line(const Point &p1, const Point &p2);

    Bool intersect(const Real &a, const Real &b, const Real &c) const;

    Bool inside(const Point &c, const Real &r) const;

    Bool operator()(const Point &p) const;
};

struct Curve
{
    Real a;
    Real b;
    Real c;

    Curve(const Point &p1, const Point &p2, const Point &p3);
    Curve(const Real &a, const Point &p1, const Point &p2);

    Real operator()(const Real &x) const;
};

#endif