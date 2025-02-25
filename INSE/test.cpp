#include <Eigen/Eigen>

using Int = long long int;
using Real = double;
using Matrix = Eigen::Matrix<Real, Eigen::Dynamic, Eigen::Dynamic>;
using Vector = Eigen::Vector<Real, Eigen::Dynamic>;

constexpr Int ns = 66;
constexpr Real s_min = 0.0;
constexpr Real s_max = 2.0;
constexpr Real ds = (s_max - s_min) / (ns - 1);

constexpr Real rho = 1.0;
constexpr Real nu = 0.1;
constexpr Real dt = 0.001;

const Matrix set_pressure_boundary(const Matrix &p)
{
    Matrix res = p;
    for (Int i = 0; i < ns; i++)
    {
        res(i, ns - 1) = res(i, ns - 2);
        res(0, i) = res(1, i);
        res(i, 0) = res(i, 1);
        res(ns - 1, i) = 0.0;
    }
    return res;
}

const Matrix diff_1st_x(const Matrix &f)
{
    Matrix res = Matrix::Zero(ns, ns);
    for (Int i = 1; i < (ns - 1); i++)
    {
        for (Int j = 1; j < (ns - 1); j++)
        {
            res(i, j) = (f(i, j + 1) - f(i, j - 1)) / (2 * ds);
        }
    }
    return res;
}

const Matrix diff_1st_y(const Matrix &f)
{
    Matrix res = Matrix::Zero(ns, ns);
    for (Int i = 1; i < (ns - 1); i++)
    {
        for (Int j = 1; j < (ns - 1); j++)
        {
            res(i, j) = (f(i + 1, j) - f(i - 1, j)) / (2 * ds);
        }
    }
    return res;
}

const Matrix forward_diff_x(const Matrix &f)
{
    Matrix res = Matrix::Zero(ns, ns);
    for (Int i = 1; i < (ns - 1); i++)
    {
        for (Int j = 1; j < (ns - 1); j++)
        {
            res(i, j) = (f(i, j + 1) - f(i, j)) / (2 * ds);
        }
    }
    return res;
}

const Matrix forward_diff_y(const Matrix &f)
{
    Matrix res = Matrix::Zero(ns, ns);
    for (Int i = 1; i < (ns - 1); i++)
    {
        for (Int j = 1; j < (ns - 1); j++)
        {
            res(i, j) = (f(i + 1, j) - f(i, j)) / (2 * ds);
        }
    }
    return res;
}

const Matrix laplacian(const Matrix &f)
{
    Matrix res = Matrix::Zero(ns, ns);
    for (Int i = 1; i < (ns - 1); i++)
    {
        for (Int j = 1; j < (ns - 1); j++)
        {
            res(i, j) = (f(i + 1, j) + f(i - 1, j) + f(i, j + 1) + f(i, j - 1) - 4.0 * f(i, j)) / (ds * ds);
        }
    }
    return res;
}

const std::pair<Matrix, Matrix> vel_without_pressure(const Matrix &u, const Matrix &v)
{
    Matrix u_star = u;
    Matrix v_star = v;
    Matrix fdxu = forward_diff_x(u);
    Matrix fdxv = forward_diff_x(v);
    Matrix fdyu = forward_diff_y(u);
    Matrix fdyv = forward_diff_y(v);
    Matrix lapu = laplacian((u));
    Matrix lapv = laplacian((v));
    for (Int i = 1; i < (ns - 1); i++)
    {
        for (Int j = 1; j < (ns - 1); j++)
        {
            u_star(i, j) -= (dt * (u(i, j) * fdxu(i, j) + v(i, j) * fdyu(i, j) + nu * lapu(i, j)));
            v_star(i, j) -= (dt * (u(i, j) * fdxv(i, j) + v(i, j) * fdyv(i, j) + nu * lapv(i, j)));
        }
    }
    return std::make_pair(u_star, v_star);
}

const Matrix get_R(const Matrix &u, const Matrix &v)
{
    return rho * (forward_diff_x(u) + forward_diff_y(v)) / dt;
}

const Matrix get_pressure(const Matrix &p, const Matrix &R)
{
    Matrix res = p;
    for (Int i = 1; i < (ns - 1); i++)
    {
        for (Int j = 1; j < (ns - 1); j++)
        {
            res(i, j) = (p(i + 1, j) + p(i - 1, j) + p(i, j + 1) + p(i, j - 1)) / 4.0 - R(i, j) * ds * ds / 4;
        }
    }
    set_pressure_boundary(res);
    return res;
}

const std::pair<Matrix, Matrix> update_velocity(const Matrix &u_star, const Matrix &v_star, const Matrix &p)
{
    Matrix u = Matrix::Zero(ns, ns);
    Matrix v = Matrix::Zero(ns, ns);
    Matrix d1xp = diff_1st_x(p);
    Matrix d1yp = diff_1st_y(p);
    for (Int i = 1; i < (ns - 1); i++)
    {
        for (Int j = 1; j < (ns - 1); j++)
        {
            u(i, j) = (-dt * d1xp(i, j) / rho + u_star(i, j));
            v(i, j) = (-dt * d1yp(i, j) / rho + v_star(i, j));
        }
    }
    return std::make_pair(u, v);
}

const std::tuple<Matrix, Matrix, Matrix> simulate_cavity_flow(const Matrix &nt, const Matrix &u, const Matrix &v, const Matrix &p)
{
    std::pair<Matrix, Matrix> star;
    for (Int i = 0; i < ns; i++)
    {
        star = vel_without_pressure(u, v);
        Matrix R = get_R(star.first, star.second);
        Matrix p = get_pressure(p, R);
        star = update_velocity(star.first, star.second, p);
    }
    return std::make_tuple(star.first, star.second, p);
}

int main()
{
    for (Int i = 0; i < ns; i++)
    {
        u(ns - 1, i) = 1.0;
    }

    simulate_cavity_flow(1000, u, v, p);

    return 0;
}