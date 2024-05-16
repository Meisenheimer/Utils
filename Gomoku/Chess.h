#ifndef MW_CHESS_H
#define MW_CHESS_H

#include "Config.h"

class Chess;

class Chess
{
private:
    Byte _color;
    Real _score_white;
    Real _score_black;
    Byte status[HEIGHT][WIDTH];

public:
    const Byte &color;
    const Real &score_white;
    const Real &score_black;

public:
    Chess();
    Chess(const Chess &chess);

    void setPawn(const Integer &h, const Integer &w);

    Bool judge(const Byte &color) const;
    Byte judge() const;

    Real white() const;
    Real black() const;
    Real score() const;

    Chess &operator=(const Chess &chess);

    Byte operator()(const Integer &h, const Integer &w) const;
    Byte &operator()(const Integer &h, const Integer &w);
};

#endif
