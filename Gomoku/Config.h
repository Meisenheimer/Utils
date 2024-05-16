#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <random>
#include <chrono>

using Bool = bool;
using Byte = unsigned char;
using Integer = long int;
using Real = double;

constexpr Bool WIN = true;

constexpr Byte EMPTY = 0;
constexpr Byte BLACK_PAWN = 1;
constexpr Byte WHITE_PAWN = 2;

constexpr Byte UNFINISHED = -1;
constexpr Byte EVEN = EMPTY;
constexpr Byte BLACK_WIN = BLACK_PAWN;
constexpr Byte WHITE_WIN = WHITE_PAWN;

constexpr Byte INITIATIVE = BLACK_PAWN;

constexpr Integer HEIGHT = 15;
constexpr Integer WIDTH = 15;

constexpr Integer GRID_SIZE = 32;

constexpr Integer NUM_WIN = 5;

constexpr Real INF = (Real)std::numeric_limits<float>::max();

const char COLOR_NAME[3][6] = {"Empty", "Black", "White"};

class Random;

void check(const Integer &h, const Integer &w);
Bool inside(const Integer &h, const Integer &w);
Byte opponent(const Byte &color);

class Random
{
private:
    std::default_random_engine random_engine;

public:
    Random();
    Integer operator()(const Integer &min, const Integer &max);
};

#endif
