#ifndef SCENE_H
#define SCENE_H

#include <list>
#include <windows.h>
// #include <conio.h>

#include <graphics.h>

#include "Config.h"

namespace mw
{
    class Scene;

    inline Real RUN_SPEED = 1;
    inline Real MPD = 1.0;

    inline Bool key_down(const int &key);
    inline Int convert(const Real &x);

    class Scene
    {
    private:
        Bool is_run;

        Vector pos_missile;
        Vector pos_target;
        Vector v_missile;
        Vector v_target;
        Vector a_missile;
        Vector a_target;

        std::list<Vector> pos_missile_prev;
        std::list<Vector> pos_target_prev;
        std::list<Vector> v_missile_prev;
        std::list<Vector> v_target_prev;

        Real time;

    private:
        void display() const;
        void step_target();
        void step_missile();
        void move();

        Bool check() const;

        const Vector target_strategy() const;
        const Vector missile_strategy() const;

    public:
        Scene();
        ~Scene();

        void run();
    };

    inline Bool key_down(const int &key)
    {
        return ((GetAsyncKeyState(key) & 0x8000) ? true : false);
    }

    inline Int convert(const Real &x)
    {
        return (Int)std::round(x / MPD);
    }

    inline void Scene::display() const
    {
        Vector prev_pos;

        cleardevice();

        setlinecolor(WHITE);
        circle(convert(pos_missile(0)), convert(pos_missile(1)), convert(config.DAMAGE_RADIUS));

        setlinecolor(RED);
        setfillcolor(RED);
        solidcircle(convert(pos_missile(0)), convert(pos_missile(1)), config.MISSILE_RADIUS);
        prev_pos = pos_missile;
        for (auto it = pos_missile_prev.begin(); it != pos_missile_prev.end(); it++)
        {
            const Vector &tmp = *it;
            line(convert(prev_pos(0)), convert(prev_pos(1)), convert(tmp(0)), convert(tmp(1)));
            solidcircle(convert(tmp(0)), convert(tmp(1)), config.MISSILE_RADIUS / 2);
            prev_pos = tmp;
        }

        setlinecolor(YELLOW);
        setfillcolor(YELLOW);
        solidcircle(convert(pos_target(0)), convert(pos_target(1)), config.TARGET_RADIUS);
        prev_pos = pos_target;
        for (auto it = pos_target_prev.begin(); it != pos_target_prev.end(); it++)
        {
            const Vector &tmp = *it;
            line(convert(prev_pos(0)), convert(prev_pos(1)), convert(tmp(0)), convert(tmp(1)));
            solidcircle(convert(tmp(0)), convert(tmp(1)), config.TARGET_RADIUS / 2);
            prev_pos = tmp;
        }

        setlinecolor(WHITE);
        setfillcolor(WHITE);
        line(convert(pos_missile(0)), convert(pos_missile(1)),
             convert(pos_missile(0) + v_missile(0)), convert(pos_missile(1) + v_missile(1)));
        line(convert(pos_target(0)), convert(pos_target(1)),
             convert(pos_target(0) + v_target(0)), convert(pos_target(1) + v_target(1)));

        setlinecolor(GREEN);
        setfillcolor(GREEN);
        line(convert(pos_missile(0)), convert(pos_missile(1)),
             convert(pos_missile(0) + a_missile(0)), convert(pos_missile(1) + a_missile(1)));
        line(convert(pos_target(0)), convert(pos_target(1)),
             convert(pos_target(0) + a_target(0)), convert(pos_target(1) + a_target(1)));

        const Int TEXT_HEIGHT = textheight("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890,.:;");

        char str[256];
        sprintf(str, "Time = %lf (%lf); Scale = [%lf, %lf]; MPD = %lf; Size = %lf x %lf; Speed = %d;",
                (double)time, (double)config.TIME_STEP, (double)config.MIN_SCALE, (double)config.MAX_SCALE,
                (double)MPD, (double)(config.WIDTH * MPD / 2), (double)(config.HEIGHT * MPD / 2), (int)RUN_SPEED);
        settextcolor(WHITE);
        outtextxy(0, 0, str);

        sprintf(str, "Missile: p = (%lf, %lf); v = (%lf, %lf), |v| = %lf / %lf; a = (%lf, %lf), |a| = %lf / %lf.",
                (double)pos_missile(0), (double)pos_missile(1), (double)v_missile(0), (double)v_missile(1),
                (double)v_missile.lpNorm<2>(), (double)config.MISSILE_MAX_SPEED,
                (double)a_missile(0), (double)a_missile(1), (double)a_missile.lpNorm<2>(), (double)config.MISSILE_MAX_ACCELERATION);
        settextcolor(RED);
        outtextxy(0, 3 * TEXT_HEIGHT, str);

        sprintf(str, "Target: p = (%lf, %lf); v = (%lf, %lf), |v| = %lf / %lf; a = (%lf, %lf), |a| = %lf / %lf;",
                (double)pos_target(0), (double)pos_target(1), (double)v_target(0), (double)v_target(1),
                (double)v_target.lpNorm<2>(), (double)config.TARGET_MAX_SPEED,
                (double)a_target(0), (double)a_target(1), (double)a_target.lpNorm<2>(), (double)config.TARGET_MAX_ACCELERATION);
        settextcolor(YELLOW);
        outtextxy(0, int(1.5 * TEXT_HEIGHT), str);

        delay_ms(0);
        // delay_fps(60);
        return;
    }

    inline void Scene::step_target()
    {
        pos_target_prev.push_front(pos_target);
        v_target_prev.push_front(v_target);
        while ((Int)pos_target_prev.size() > config.MAX_SAVED)
        {
            pos_target_prev.pop_back();
        }
        while ((Int)v_target_prev.size() > config.MAX_SAVED)
        {
            v_target_prev.pop_back();
        }
        a_target = target_strategy();
        if (a_target.lpNorm<2>() > 1)
        {
            a_target /= a_target.lpNorm<2>();
            a_target *= config.TARGET_MAX_ACCELERATION;
        }
        const Real t = (RUN_SPEED * config.TIME_STEP);
        pos_target += t * (v_target + t * a_target / 2);
        v_target += a_target * t;
        return;
    }

    inline void Scene::step_missile()
    {
        pos_missile_prev.push_front(pos_missile);
        v_missile_prev.push_front(v_missile);
        while ((Int)pos_missile_prev.size() > config.MAX_SAVED)
        {
            pos_missile_prev.pop_back();
        }
        while ((Int)v_missile_prev.size() > config.MAX_SAVED)
        {
            v_missile_prev.pop_back();
        }
        a_missile = missile_strategy();
        if (a_missile.lpNorm<2>() > 1)
        {
            a_missile /= a_missile.lpNorm<2>();
            a_missile *= config.MISSILE_MAX_ACCELERATION;
        }
        const Real t = (RUN_SPEED * config.TIME_STEP);
        pos_missile += t * (v_missile + t * a_missile / 2);
        v_missile += a_missile * t;
        return;
    }

    inline void Scene::move()
    {
        const Real dist = (pos_missile - pos_target).lpNorm<2>();
        const Int M = std::min(config.HEIGHT, config.WIDTH);
        Real scale = dist / (MPD * M);
        if (scale < config.MIN_SCALE)
        {
            MPD = dist / (config.MIN_SCALE * M);
        }
        else if (scale > config.MAX_SCALE)
        {
            MPD = dist / (config.MAX_SCALE * M);
        }

        Vector offset = (pos_missile + pos_target) / 2.0;
        offset(0) -= (config.WIDTH * MPD / 2.0);
        offset(1) -= (config.HEIGHT * MPD / 2.0);

        pos_missile -= offset;
        pos_target -= offset;
        for (auto it = pos_missile_prev.begin(); it != pos_missile_prev.end(); it++)
        {
            *it -= offset;
        }
        for (auto it = pos_target_prev.begin(); it != pos_target_prev.end(); it++)
        {
            *it -= offset;
        }
        if (v_target.lpNorm<2>() > config.TARGET_MAX_SPEED)
        {
            v_target /= v_target.lpNorm<2>();
            v_target *= config.TARGET_MAX_SPEED;
        }
        if (v_missile.lpNorm<2>() > config.MISSILE_MAX_SPEED)
        {
            v_missile /= v_missile.lpNorm<2>();
            v_missile *= config.MISSILE_MAX_SPEED;
        }
        return;
    }

    inline Bool Scene::check() const
    {
        if ((pos_missile - pos_target).lpNorm<2>() <= config.DAMAGE_RADIUS)
        {
            return true;
        }
        if (time >= config.END_TIME)
        {
            return true;
        }
        return false;
    }

    inline Scene::Scene()
    {
        pos_missile(0) = config.MISSILE_START_X;
        pos_missile(1) = config.MISSILE_START_Y;
        pos_target(0) = config.TARGET_START_X;
        pos_target(1) = config.TARGET_START_Y;

        v_missile(0) = config.MISSILE_START_V_X;
        v_missile(1) = config.MISSILE_START_V_Y;
        v_target(0) = config.TARGET_START_V_X;
        v_target(1) = config.TARGET_START_V_Y;

        a_missile = Vector::Zero();
        a_target = Vector::Zero();

        is_run = false;
        time = 0.0;

        const Real dist = (pos_missile - pos_target).lpNorm<2>();
        const Int M = std::min(config.HEIGHT, config.WIDTH);
        MPD = 2.0 * dist / ((config.MIN_SCALE + config.MAX_SCALE) * M);

        initgraph(config.WIDTH, config.HEIGHT, INIT_RENDERMANUAL);
        close_console();
    }

    inline Scene::~Scene()
    {
        closegraph();
    }

    inline void Scene::run()
    {
        move();
        display();
        while (true)
        {
            if (check())
            {
                getch();
                return;
            }
            if (key_down('q') || key_down('Q'))
            {
                getch();
                return;
            }
            if (key_down('c') || key_down('C') || key_down(13))
            {
                is_run = true;
            }
            if (key_down('p') || key_down('P') || key_down(' '))
            {
                is_run = false;
            }
            for (Int i = 1; i <= 9; i++)
            {
                if (key_down('0' + i))
                {
                    RUN_SPEED = i;
                }
            }
            if (key_down(',') || key_down('<'))
            {
                RUN_SPEED /= 2;
                if (RUN_SPEED < 0.1)
                {
                    RUN_SPEED = 0.1;
                }
            }
            if (key_down('.') || key_down('>'))
            {
                RUN_SPEED *= 2;
                if (RUN_SPEED > 9)
                {
                    RUN_SPEED = 9;
                }
            }
            if (is_run)
            {
                time += (RUN_SPEED * config.TIME_STEP);
                step_missile();
                step_target();
                move();
            }
            display();
        }
    }
};

#include "TargetStrategy.h"
#include "MissileStrategy.h"

#endif