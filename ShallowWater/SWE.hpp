#ifndef MW_SWE_HPP
#define MW_SWE_HPP

#include <iostream>

#include "SWE.h"

namespace mw
{
    Real g_h_h(const Real &h, const Real &u)
    {
        return u;
    }

    Real g_h_u(const Real &h, const Real &u)
    {
        return h;
    }

    Real g_u_h(const Real &h, const Real &u)
    {
        return G;
    }

    Real g_u_u(const Real &h, const Real &u)
    {
        return u;
    }

    Real f_h(const Real &h, const Real &u)
    {
        return h * u;
    }

    Real f_u(const Real &h, const Real &u)
    {
        return u * u / 2.0 + G * h;
    }

    SWE::SWE()
    {
        t = 0;
        h.resize(TOTAL_SIZE);
        u.resize(TOTAL_SIZE);
        init();
    }

    void SWE::step()
    {
        // Richtmyer.
        // List<Real> h_face(GRID_SIZE + 1, 0);
        // List<Real> u_face(GRID_SIZE + 1, 0);
        // for (Int i = 0; i <= X_MAX; i++)
        // {
        //     h_face.at(i) = (h.at(i) + h.at(i + 1)) / 2.0 -
        //                    DT * (f_h(h.at(i + 1), u.at(i + 1)) - f_h(h.at(i), u.at(i))) / (2.0 * DX);
        //     u_face.at(i) = (u.at(i) + u.at(i + 1)) / 2.0 -
        //                    DT * (f_u(h.at(i + 1), u.at(i + 1)) - f_u(h.at(i), u.at(i))) / (2.0 * DX);
        // }
        // h_face.front() = h.front();
        // h_face.back() = h.back();
        // u_face.front() = u.front();
        // u_face.back() = u.back();
        // for (Int i = X_MIN; i <= X_MAX; i++)
        // {
        //     h.at(i) -= DT * (f_h(h_face.at(i), u_face.at(i)) - f_h(h_face.at(i - 1), u_face.at(i - 1))) / DX;
        //     u.at(i) -= DT * (f_u(h_face.at(i), u_face.at(i)) - f_u(h_face.at(i - 1), u_face.at(i - 1))) / DX;
        // }
        // Lax-Friedrichs
        auto old_h = h;
        auto old_u = u;
        // #pragma omp parallel for
        for (Int i = X_MIN; i <= X_MAX; i++)
        {
            h.at(i) = (old_h.at(i + 1) + old_h.at(i - 1)) / 2.0 -
                      DT * (f_h(old_h.at(i + 1), old_u.at(i + 1)) - f_h(old_h.at(i - 1), old_u.at(i - 1))) / (2.0 * DX);
            u.at(i) = (old_u.at(i + 1) + old_u.at(i - 1)) / 2.0 -
                      DT * (f_u(old_h.at(i + 1), old_u.at(i + 1)) - f_u(old_h.at(i - 1), old_u.at(i - 1))) / (2.0 * DX);
        }
        // Lax-Wendroff
        // List<Matrix> a(GRID_SIZE + 1, Matrix::Zero(2, 2));
        // List<Vector> f(TOTAL_SIZE, Vector::Zero(2));
        // for (Int i = 0; i <= X_MAX; i++)
        // {
        //     Real tmp_h = (h.at(i) + h.at(i + 1)) / 2.0;
        //     Real tmp_u = (u.at(i) + u.at(i + 1)) / 2.0;
        //     a.at(i)(0, 0) = g_h_h(tmp_h, tmp_u);
        //     a.at(i)(0, 1) = g_h_u(tmp_h, tmp_u);
        //     a.at(i)(1, 0) = g_u_h(tmp_h, tmp_u);
        //     a.at(i)(1, 1) = g_u_u(tmp_h, tmp_u);
        // }
        // for (Int i = 0; i < TOTAL_SIZE; i++)
        // {
        //     f.at(i)(0) = f_h(h.at(i), u.at(i));
        //     f.at(i)(1) = f_u(h.at(i), u.at(i));
        // }
        // for (Int i = X_MIN; i <= X_MAX; i++)
        // {
        //     Vector hu = Vector::Zero(2);
        //     hu = -DT * (f.at(i + 1) - f.at(i - 1)) / (2.0 * DX);
        //     hu += DT * DT * (a.at(i) * (f.at(i + 1) - f.at(i)) - a.at(i - 1) * (f.at(i) - f.at(i - 1))) / (2.0 * DX * DX);
        //     h.at(i) += hu(0);
        //     u.at(i) += hu(1);
        // }
        t++;
        boundary();
    }

    void SWE::save() const
    {
        FILE *fp = fopen("./res/h.dat", "w");
        assert(fp != nullptr);
        for (Int i = 0; i < TOTAL_SIZE; i++)
        {
            fprintf(fp, "%Lf %Lf 0.0\n", DX * i - DX / 2, h.at(i));
        }
        fclose(fp);
        fp = fopen("./res/u.dat", "w");
        assert(fp != nullptr);
        for (Int i = 0; i < TOTAL_SIZE; i++)
        {
            fprintf(fp, "%Lf %Lf\n", DX * i - DX / 2, u.at(i) / h.at(i));
        }
        fclose(fp);

        FILE *pipe = popen("gnuplot", "w");
        assert(pipe != nullptr);
        fprintf(pipe, "set terminal png size %d, %d\n", 640, 480);
        fprintf(pipe, "set output './res/h_%.1Lf.png'\n", (Real)t / DT_PER_SECOND);
        fprintf(pipe, "set title 't = %4.1Lf s'\n", (Real)t / DT_PER_SECOND);
        fprintf(pipe, "unset key\n");
        fprintf(pipe, "set xrange[0.0: %Lf]\n", SIZE);
        fprintf(pipe, "set yrange[0.0: 10.0]\n");
        fprintf(pipe, "plot \"./res/h.dat\" using 1:2 with lines, \"./res/h.dat\" using 1:2:3 with filledcurves fc rgb \"#3080F0\"\n");
        fprintf(pipe, "unset output\n");

        // fprintf(pipe, "set terminal png size %d, %d\n", 640, 480);
        // fprintf(pipe, "set output './res/u_%.1Lf.png'\n", (Real)t / DT_PER_SECOND);
        // fprintf(pipe, "unset title\n");
        // fprintf(pipe, "unset key\n");
        // fprintf(pipe, "set xrange[0.0: %Lf]\n", SIZE);
        // fprintf(pipe, "set yrange[-1.0: 1.0]\n");
        // fprintf(pipe, "plot \"./res/u.dat\" with lines\n");
        // fprintf(pipe, "unset output\n");
        pclose(pipe);

        return;
    }
};

#endif