#ifndef CONIFG_H
#define CONIFG_H

#include <Eigen/Eigen>

#define MW_ALERT printf("%s %d\n", __FILE__, __LINE__);

using Bool = bool;
using Integer = long int;
using Real = double;
using String = std::string;

using Vector = Eigen::Vector<Real, 2>;
using Point = Vector;

constexpr Integer WIDTH = 800;
constexpr Integer HEIGHT = 800;
constexpr Integer SCALE = 40;

constexpr Integer STEP = 200;

constexpr Real DAMAGE_RADIUS = 1.0;
constexpr Real BALL_RADIUS = 0.3;

constexpr Real EPSILON = std::numeric_limits<float>::epsilon();
constexpr Real DELTA = std::numeric_limits<float>::epsilon();
constexpr Real INF = std::numeric_limits<float>::max();

#endif