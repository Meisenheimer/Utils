#ifndef MW_CDE_HPP
#define MW_CDE_HPP

#include "CDE.h"

namespace mw
{
    CDE::CDE(const Matrix &u, const Matrix &v)
    {
        t = 0;
        this->u = u;
        this->v = v;
        f = Matrix::Zero(GRID_SIZE - 1, GRID_SIZE - 1);
        r = Matrix::Zero(GRID_SIZE - 1, GRID_SIZE - 1);

        init();
    }

    void CDE::step()
    {
        auto F = [&u = std::as_const(u), &v = std::as_const(v), &r = std::as_const(r)](const Matrix &f) -> const Matrix
        {
            Matrix res = Matrix::Zero(GRID_SIZE - 1, GRID_SIZE - 1);
            for (Int i = 0; i < MAX; i++)
            {
                for (Int j = 0; j < MAX; j++)
                {
                    if (i - 1 >= 0)
                    {
                        res(i, j) += MU * f(i - 1, j);
                        res(i, j) -= MU * f(i, j);
                        Real tmp = DS * (u(i, j) + u(i, j + 1)) / 2.0;
                        // res(i, j) -= tmp * (f(i, j) - f(i - 1, j));
                        if (tmp >= 0)
                        {
                            res(i, j) += (f(i - 1, j) * tmp);
                        }
                        else
                        {
                            res(i, j) += (f(i, j) * tmp);
                        }
                    }
                    if (j - 1 >= 0)
                    {
                        res(i, j) += MU * f(i, j - 1);
                        res(i, j) -= MU * f(i, j);
                        Real tmp = DS * (v(i + 1, j) + v(i, j)) / 2.0;
                        // res(i, j) -= tmp * (f(i, j) - f(i, j - 1));
                        if (tmp >= 0)
                        {
                            res(i, j) += (f(i, j - 1) * tmp);
                        }
                        else
                        {
                            res(i, j) += (f(i, j) * tmp);
                        }
                    }

                    if (i + 1 < MAX)
                    {
                        res(i, j) += MU * f(i + 1, j);
                        res(i, j) -= MU * f(i, j);
                        Real tmp = DS * (u(i + 1, j) + u(i + 1, j + 1)) / 2.0;
                        // res(i, j) -= tmp * (f(i + 1, j) - f(i, j));
                        if (tmp >= 0)
                        {
                            res(i, j) -= (f(i, j) * tmp);
                        }
                        else
                        {
                            res(i, j) -= (f(i + 1, j) * tmp);
                        }
                    }
                    if (j + 1 < MAX)
                    {
                        res(i, j) += MU * f(i, j + 1);
                        res(i, j) -= MU * f(i, j);
                        Real tmp = DS * (v(i + 1, j + 1) + v(i, j + 1)) / 2.0;
                        // res(i, j) -= tmp * (f(i, j + 1) - f(i, j));
                        if (tmp >= 0)
                        {
                            res(i, j) -= (f(i, j) * tmp);
                        }
                        else
                        {
                            res(i, j) -= (f(i, j + 1) * tmp);
                        }
                    }
                }
            }
            res /= (DS * DS);
            res += r;
            return res;
            // return (MU * lap(f) - u.cwiseProduct(c1x(f)) - v.cwiseProduct(c1y(f)) + r);
            // return (MU * lap(f) - adux(f, u) - aduy(f, v) + r);
        };
        // std::cout << f << std::endl;
        // std::cout << F(f) << std::endl
        //           << std::endl;
        // getchar();
        // f += (DT * F(f));

        Matrix y1 = f + DT * F(f);
        Matrix y2 = (3.0 * f + y1 + DT * F(y1)) / 4.0;
        f = (f + 2.0 * y2 + 2.0 * DT * F(y2)) / 3.0;

        for (Int i = MIN; i < MAX; i++)
        {
            for (Int j = MIN; j < MAX; j++)
            {
                if (f(i, j) < 0.0)
                {
                    f(i, j) = 0.0;
                }
                if (f(i, j) > 100.0)
                {
                    f(i, j) = 100.0;
                }
            }
        }

        boundary();
        t++;
        return;
    }

    bool CDE::check() const
    {
        for (Int i = MIN; i < MAX; i++)
        {
            for (Int j = MIN; j < MAX; j++)
            {
                if (std::isnan(f(i, j)))
                {
                    return true;
                }
            }
        }
        return false;
    }

    void CDE::print(std::ostream &out) const
    {
        out << "t" << std::endl;
        out << t << std::endl;
        out << "F" << std::endl;
        out << f << std::endl;
        return;
    }

    void CDE::save() const
    {
        FILE *pipe = popen("gnuplot", "w");
        assert(pipe != nullptr);
        fprintf(pipe, "set terminal png size %d, %d\n", 2 * 640, 2 * 480);
        fprintf(pipe, "set output './res/%lld.png'\n", t);
        fprintf(pipe, "set title '%8.2lf%%'\n", 100 * (double)t / ITERATION_CDE);
        fprintf(pipe, "unset key\n");
        fprintf(pipe, "set xrange[0.6: %lld.6]\n", MAX - 2);
        fprintf(pipe, "set yrange[0.6: %lld.6]\n", MAX - 2);
        fprintf(pipe, "set cbrange[0.0: 100.0]\n");
        fprintf(pipe, "$map1 << EOD\n");
        for (Int j = MIN; j < MAX; j++)
        {
            for (Int i = MIN; i < MAX; i++)
            {
                fprintf(pipe, "%lf ", (double)f(i, j));
            }
            fprintf(pipe, "\n");
        }
        fprintf(pipe, "EOD\n");
        fprintf(pipe, "set view map\n");
        fprintf(pipe, "splot '$map1' matrix with image\n");
        fprintf(pipe, "unset output\n");
        pclose(pipe);
        return;
    }
}

#endif
