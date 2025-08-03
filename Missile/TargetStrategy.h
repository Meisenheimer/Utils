#ifndef TARGET_STRATEGY_H
#define TARGET_STRATEGY_H

#include "Scene.h"
#include "Config.h"

namespace mw
{
    inline const Vector Scene::target_strategy() const
    {
        const Real T = (RUN_SPEED * config.TIME_STEP);
        constexpr Real lambda = 20;

        const Vector u = v_target / v_target.lpNorm<2>();
        Vector un;
        un(0) = -u(1);
        un(1) = u(0);
        Vector vn;
        vn(0) = -v_missile(1);
        vn(1) = v_missile(0);
        const Vector t = vn.dot(u) * vn / vn.lpNorm<2>();
        const Vector n = un.dot(t) * un;

        Vector res = lambda * n + u + pos_target - (pos_missile + v_missile * T + a_missile * T * T / 2);
        // Vector res = pos_target - (pos_missile + v_missile * T + a_missile * T * T / 2);
        res /= res.lpNorm<2>();
        res *= config.TARGET_MAX_ACCELERATION;
        return res;
    }
};

#endif