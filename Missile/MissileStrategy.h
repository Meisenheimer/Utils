#ifndef MISSILE_STRATEGY_H
#define MISSILE_STRATEGY_H

#include "Scene.h"
#include "Config.h"

namespace mw
{
    inline const Vector Scene::missile_strategy() const
    {
        // const Real T = 0.0;
        const Real T = (RUN_SPEED * config.TIME_STEP);
        // constexpr Real lambda = 10;
        // const Vector u = v_missile / v_missile.lpNorm<2>();
        // const Vector v = (pos_target + v_target * T + a_target * T * T / 2) - pos_missile;
        // const Vector tmp = u.dot(v) * u;

        // Vector res = lambda * v - (lambda - 1) * tmp;
        Vector res = (pos_target + v_target * T + a_target * T * T / 2) - pos_missile;
        res /= res.lpNorm<2>();
        res *= config.MISSILE_MAX_ACCELERATION;
        return res;
    }
};

#endif