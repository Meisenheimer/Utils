#ifndef MW_CDE_H
#define MW_CDE_H

#include <iostream>

#include "Config.h"

namespace mw
{
    class CDE;

    class CDE
    {
    private:
        Int t;

        Matrix u;
        Matrix v;

        Matrix f;

        Matrix r;

    private:
        void init();
        void boundary();

    public:
        CDE(const Matrix &u, const Matrix &v);

        void step();

        bool check() const;
        void print(std::ostream &out = std::cout) const;
        void save() const;
    };
}

#include "CDE.hpp"

#endif