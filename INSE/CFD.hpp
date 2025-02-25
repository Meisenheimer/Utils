#ifndef MW_CFD_HPP
#define MW_CFD_HPP

#include "CFD.h"

namespace mw
{
    inline void CFD::predict()
    {
        auto F = [&fx = std::as_const(fx), &fy = std::as_const(fy)](const Matrix &u, const Matrix &v)
        {
            Matrix u_star = (fx + NU * lap(u) - udux(u) - aduy(u, v));
            Matrix v_star = (fy + NU * lap(v) - adux(v, u) - uduy(v));
            return std::make_pair(std::move(u_star), std::move(v_star));
        };
        auto y1 = F(u, v);
        y1.first = u + DT * y1.first;
        y1.second = v + DT * y1.second;
        auto y2 = F(y1.first, y1.second);
        y2.first = (3.0 * u + y1.first + DT * y2.first) / 4.0;
        y2.second = (3.0 * v + y1.second + DT * y2.second) / 4.0;
        auto y = F(y2.first, y2.second);
        u = (u + 2.0 * y2.first + 2.0 * DT * y.first) / 3.0;
        v = (v + 2.0 * y2.second + 2.0 * DT * y.second) / 3.0;
        return;
    }

    inline void CFD::correct()
    {
        Matrix R = DS * DS * RHO * (c1x(u) + c1y(v)) / DT;
        for (Int relax_time = 0; relax_time < RELAX_TIME; relax_time++)
        {
            for (Int i = MIN + 1; i < MAX; i++)
            {
                for (Int j = MIN + 1; j < MAX; j++)
                {
                    p(i, j) = (p(i + 1, j) + p(i - 1, j) + p(i, j + 1) + p(i, j - 1) - R(i, j)) / 4.0;
                }
            }
            boundary_p();
        }

        u -= (DT / RHO) * c1x(p);
        v -= (DT / RHO) * c1y(p);
        return;
    }

    inline CFD::CFD()
    {
        t = 0;
        u = Matrix::Zero(GRID_SIZE, GRID_SIZE);
        v = Matrix::Zero(GRID_SIZE, GRID_SIZE);
        p = Matrix::Zero(GRID_SIZE, GRID_SIZE);
        fx = Matrix::Zero(GRID_SIZE, GRID_SIZE);
        fy = Matrix::Zero(GRID_SIZE, GRID_SIZE);
        init();
    }

    inline void CFD::step()
    {
        boundary_uv();
        predict();
        correct();
        t++;
        return;
    }

    inline void CFD::save() const
    {
        FILE *fp = fopen("./res.dat", "w");
        assert(fp != nullptr);
        for (Int j = MIN; j <= MAX; j += STEP)
        {
            for (Int i = MIN; i <= MAX; i += STEP)
            {
                Real tmp_u = u(i, j);
                Real tmp_v = v(i, j);
                fprintf(fp, "%ld %ld %lf %lf\n", (long int)i, (long int)j, (double)tmp_u, (double)tmp_v);
            }
        }
        fclose(fp);

        FILE *pipe = popen("gnuplot", "w");
        assert(pipe != nullptr);
        fprintf(pipe, "set terminal png size %d, %d\n", 2 * 640, 2 * 480);
        fprintf(pipe, "set output './res_uv.png'\n");
        fprintf(pipe, "unset title\n");
        fprintf(pipe, "unset key\n");
        fprintf(pipe, "set xrange[-0.6: %lld.6]\n", GRID_SIZE - 1);
        fprintf(pipe, "set yrange[-0.6: %lld.6]\n", GRID_SIZE - 1);
        fprintf(pipe, "plot \"./res.dat\" using 1:2:3:4 with vectors linecolor black\n");
        fprintf(pipe, "unset output\n");
        pclose(pipe);

        pipe = popen("gnuplot", "w");
        assert(pipe != nullptr);
        fprintf(pipe, "set terminal png size %d, %d\n", 2 * 640, 2 * 480);
        fprintf(pipe, "set output './res_norm.png'\n");
        fprintf(pipe, "unset title\n");
        fprintf(pipe, "unset key\n");
        fprintf(pipe, "set xrange[-0.6: %lld.6]\n", GRID_SIZE - 1);
        fprintf(pipe, "set yrange[-0.6: %lld.6]\n", GRID_SIZE - 1);
        fprintf(pipe, "$map1 << EOD\n");
        for (Int j = MIN; j <= MAX; j++)
        {
            for (Int i = MIN; i <= MAX; i++)
            {
                fprintf(pipe, "%lf ", (double)std::sqrt(u(i, j) * u(i, j) + v(i, j) * v(i, j)));
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

    inline bool CFD::check() const
    {
        for (Int i = 0; i < GRID_SIZE; i++)
        {
            for (Int j = 0; j < GRID_SIZE; j++)
            {
                if (std::isnan(p(i, j)) || std::isnan(u(i, j)) || std::isnan(v(i, j)))
                {
                    return true;
                }
            }
        }
        return false;
    }

    inline void CFD::print(std::ostream &out) const
    {
        out << "t" << std::endl;
        out << t << std::endl;
        out << "u" << std::endl;
        out << u << std::endl;
        out << "v" << std::endl;
        out << v << std::endl;
        out << "p" << std::endl;
        out << p << std::endl;
        return;
    }

    inline std::pair<Matrix, Matrix> CFD::result() const
    {
        return std::make_pair(u, v);
    }
};

#endif