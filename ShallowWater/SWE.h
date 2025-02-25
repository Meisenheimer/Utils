#ifndef MW_SWE_H
#define MW_SWE_H

#include "Config.h"

namespace mw
{
    class SWE;

    Real g_h_h(const Real &h, const Real &u);
    Real g_h_u(const Real &h, const Real &u);
    Real g_u_h(const Real &h, const Real &u);
    Real g_u_u(const Real &h, const Real &u);
    Real f_h(const Real &h, const Real &u);
    Real f_u(const Real &h, const Real &u);

    class SWE
    {
    private:
        Int t;
        List<Real> h;
        List<Real> u;

    public:
        SWE();

        void init();
        void boundary();

        void step();
        void save() const;
    };
};

#include "SWE.hpp"

#endif