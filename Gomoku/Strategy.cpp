#include <vector>
#include <tuple>
#include <iostream>

#include "Strategy.h"

std::pair<Integer, Integer> blackStrategy(const Chess &chess)
{
    Integer h, w;
    scanf("%ld%ld", &h, &w);
    getchar();
    return std::make_pair(h, w);
}

std::pair<Integer, Integer> whiteStrategy(const Chess &chess)
{
    Integer h, w;
    scanf("%ld%ld", &h, &w);
    getchar();
    return std::make_pair(h, w);
}
