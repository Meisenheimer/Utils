#ifndef MW_CFD_H
#define MW_CFD_H

#include <iostream>

#include "Config.h"

namespace mw
{
    class CFD;

    class CFD
    {
    private:
        Int t;

        Matrix fx;
        Matrix fy;
        Matrix u;
        Matrix v;
        Matrix p;

    private:
        void init();
        void boundary_uv();
        void boundary_p();

    private:
        void predict();
        void correct();

    public:
        CFD();

        void step();

        void save() const;
        bool check() const;
        void print(std::ostream &out = std::cout) const;

        std::pair<Matrix, Matrix> result() const;
    };
};

#include "CFD.hpp"

#endif