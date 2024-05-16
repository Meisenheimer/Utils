#include "Config.h"

void check(const Integer &h, const Integer &w)
{
    if (h < 0 || h >= HEIGHT || w < 0 || w > WIDTH)
    {
        printf("Error at: %s %d\n", __FILE__, __LINE__);
        getchar();
        exit(0);
    }
    return;
}

Bool inside(const Integer &h, const Integer &w)
{
    return (h >= 0 && h < HEIGHT && w >= 0 && w < WIDTH);
}

Byte opponent(const Byte &color)
{
    if (color == WHITE_PAWN)
    {
        return BLACK_PAWN;
    }
    if (color == BLACK_PAWN)
    {
        return WHITE_PAWN;
    }
    printf("Error at: %s %d\n", __FILE__, __LINE__);
    getchar();
    exit(0);
}

Random::Random()
{
    Integer seed = (uint_fast32_t)time(nullptr);
    // printf("seed = %d\n", seed);
    // random_engine.seed(seed);
    random_engine.seed((uint_fast32_t)seed);
}

Integer Random::operator()(const Integer &min, const Integer &max)
{
    return std::uniform_int_distribution<Integer>(min, max)(random_engine);
}
