#ifndef CONIFG_H
#define CONIFG_H

#include <fstream>

#include <Eigen/Eigen>

#include "inipp.h"

#define MW_ALERT printf("%s %d\n", __FILE__, __LINE__);
namespace mw
{
    using Bool = bool;
    using Int = long int;
    using Real = long double;
    using String = std::string;

    using Vector = Eigen::Vector<Real, 2>;
    using Point = Vector;

    struct Config;

    constexpr Real DELTA = std::numeric_limits<float>::epsilon();

    struct Config
    {
        // [SCREEN]
        Int WIDTH = 800;
        Int HEIGHT = 640;
        Real MIN_SCALE = 0.1;
        Real MAX_SCALE = 0.9;

        // [GLOBAL]
        Int MAX_SAVED = 5;
        Real TIME_STEP = 1e-3;
        Real END_TIME = 1e-3;

        // [TARGET]
        Real TARGET_START_X = 10.0;
        Real TARGET_START_Y = 10.0;
        Real TARGET_START_V_X = 10.0;
        Real TARGET_START_V_Y = 10.0;
        Real TARGET_MAX_ACCELERATION = 1.0;
        Real TARGET_MAX_SPEED = 1.0;
        Int TARGET_RADIUS = 5;

        // [MISSILE]
        Real DAMAGE_RADIUS = 10.0;
        Real MISSILE_START_X = 10.0;
        Real MISSILE_START_Y = 10.0;
        Real MISSILE_START_V_X = 10.0;
        Real MISSILE_START_V_Y = 10.0;
        Real MISSILE_MAX_ACCELERATION = 1.0;
        Real MISSILE_MAX_SPEED = 1.0;
        Int MISSILE_RADIUS = 5;

        Config();
    };

    const Config config;

    inline Config::Config()
    {
        inipp::Ini<char> ini;
        std::ifstream is("config.ini");
        if (!is.is_open())
        {
            std::cout << "Can't open config.ini, use default settings." << std::endl;
            exit(0);
        }
        ini.parse(is);
        // [SCREEN]
        inipp::get_value(ini.sections["SCREEN"], "WIDTH", this->WIDTH);
        inipp::get_value(ini.sections["SCREEN"], "HEIGHT", this->HEIGHT);
        inipp::get_value(ini.sections["SCREEN"], "MIN_SCALE", this->MIN_SCALE);
        inipp::get_value(ini.sections["SCREEN"], "MAX_SCALE", this->MAX_SCALE);
        // [GLOBAL]
        inipp::get_value(ini.sections["GLOBAL"], "MAX_SAVED", this->MAX_SAVED);
        inipp::get_value(ini.sections["GLOBAL"], "TIME_STEP", this->TIME_STEP);
        inipp::get_value(ini.sections["GLOBAL"], "END_TIME", this->END_TIME);
        // [MISSILE]

        // [TARGET]
        inipp::get_value(ini.sections["TARGET"], "TARGET_START_X", this->TARGET_START_X);
        inipp::get_value(ini.sections["TARGET"], "TARGET_START_Y", this->TARGET_START_Y);
        inipp::get_value(ini.sections["TARGET"], "TARGET_START_V_X", this->TARGET_START_V_X);
        inipp::get_value(ini.sections["TARGET"], "TARGET_START_V_Y", this->TARGET_START_V_Y);
        inipp::get_value(ini.sections["TARGET"], "TARGET_MAX_ACCELERATION", this->TARGET_MAX_ACCELERATION);
        inipp::get_value(ini.sections["TARGET"], "TARGET_MAX_SPEED", this->TARGET_MAX_SPEED);
        inipp::get_value(ini.sections["TARGET"], "TARGET_RADIUS", this->TARGET_RADIUS);

        // [MISSILE]
        inipp::get_value(ini.sections["MISSILE"], "DAMAGE_RADIUS", this->DAMAGE_RADIUS);
        inipp::get_value(ini.sections["MISSILE"], "MISSILE_START_X", this->MISSILE_START_X);
        inipp::get_value(ini.sections["MISSILE"], "MISSILE_START_Y", this->MISSILE_START_Y);
        inipp::get_value(ini.sections["MISSILE"], "MISSILE_START_V_X", this->MISSILE_START_V_X);
        inipp::get_value(ini.sections["MISSILE"], "MISSILE_START_V_Y", this->MISSILE_START_V_Y);
        inipp::get_value(ini.sections["MISSILE"], "MISSILE_MAX_ACCELERATION", this->MISSILE_MAX_ACCELERATION);
        inipp::get_value(ini.sections["MISSILE"], "MISSILE_MAX_SPEED", this->MISSILE_MAX_SPEED);
        inipp::get_value(ini.sections["MISSILE"], "MISSILE_RADIUS", this->MISSILE_RADIUS);
        return;
    }
}

#endif