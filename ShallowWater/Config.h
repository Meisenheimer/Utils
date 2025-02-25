#ifndef MW_CONFIG_H
#define MW_CONFIG_H

#include <vector>

#include <Eigen/Eigen>

#define MW_ALERT printf("%s %d\n", __FILE__, __LINE__);

namespace mw
{
    using Int = long long int;
    using Real = long double;
    template <typename Type>
    using List = std::vector<Type>;

    using SparseMatrix = Eigen::SparseMatrix<Real>;
    using Matrix = Eigen::Matrix<Real, Eigen::Dynamic, Eigen::Dynamic>;
    using Vector = Eigen::Vector<Real, Eigen::Dynamic>;

    inline constexpr Int PADDING = 1;
    inline constexpr Int GRID_SIZE = 1000;
    inline constexpr Int TOTAL_SIZE = GRID_SIZE + 2 * PADDING;
    inline constexpr Int X_MIN = 1;
    inline constexpr Int X_MAX = GRID_SIZE + PADDING - 1;
    inline constexpr Real SIZE = 100.0;

    inline constexpr Int DT_PER_SECOND = 1000;
    inline constexpr Real DX = SIZE / GRID_SIZE;
    inline constexpr Real DT = 1.0 / DT_PER_SECOND;

    inline constexpr Real MU = DT / DX;
    inline constexpr Real G = 10.0;
};

#endif