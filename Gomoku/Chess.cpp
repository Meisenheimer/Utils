#include <iostream>
#include <memory.h>
#include <tuple>

#include "Chess.h"

Chess::Chess() : color(_color), score_white(_score_white), score_black(_score_black)
{
    _color = INITIATIVE;
    _score_white = _score_black = 0;
    memset(status, EMPTY, sizeof(status));
}

Chess::Chess(const Chess &chess) : color(_color), score_white(_score_white), score_black(_score_black)
{
    _color = chess.color;
    _score_white = chess.score_white;
    _score_black = chess.score_black;
    memcpy(status, chess.status, sizeof(status));
}

inline Real calcScore(const Chess &chess, const Integer &h, const Integer &w, const Byte &color)
{
    const std::vector<std::pair<std::vector<Bool>, Real>> &CHESS_PATTERN = {
        {{0, 1, 1, 0, 0}, 50.0},
        {{0, 0, 1, 1, 0}, 50.0},
        {{1, 1, 0, 1, 0}, 200.0},
        {{1, 0, 1, 1, 0}, 200.0},
        {{0, 1, 1, 0, 1}, 200.0},
        {{0, 1, 0, 1, 1}, 200.0},
        {{1, 1, 1, 0, 0}, 500.0},
        {{0, 0, 1, 1, 1}, 500.0},
        {{0, 1, 1, 1, 0}, 5000.0},
        {{0, 1, 0, 1, 1, 0}, 5000.0},
        {{0, 1, 1, 0, 1, 0}, 5000.0},
        {{1, 1, 1, 0, 1}, 5000.0},
        {{1, 1, 0, 1, 1}, 5000.0},
        {{1, 0, 1, 1, 1}, 5000.0},
        {{1, 1, 1, 1, 0}, 5000.0},
        {{0, 1, 1, 1, 1}, 5000.0},
        {{0, 1, 1, 1, 1, 0}, 50000.0},
        {{1, 1, 1, 1, 1}, 1.0e10}};
    const Integer DIRECTION[][2] = {{0, 1}, {1, 0}, {1, 1}, {-1, 1}};
    const Byte opponent_color = opponent(color);
    Real sum = 0.0;
    for (Integer i = 0; i < (Integer)(sizeof(DIRECTION) / sizeof(DIRECTION[0])); i++)
    {
        Real max_score = 0.0;
        const Integer dx = DIRECTION[i][0], dy = DIRECTION[i][1];
        for (Integer index = 0; index < (Integer)CHESS_PATTERN.size(); index++)
        {
            const std::vector<Bool> pattern = CHESS_PATTERN[index].first;
            const Real pattern_score = CHESS_PATTERN[index].second;
            const Integer len = pattern.size();
            for (Integer offset = 1 - len; offset < len; offset++)
            {
                Integer x = h + offset * dx, y = w + offset * dy;
                Bool matching = true;
                for (Integer k = 0; k < len; k++)
                {
                    if (!inside(x, y) || (!pattern[k] && chess(x, y) != EMPTY) || (pattern[k] && chess(x, y) != color))
                    {
                        matching = false;
                        break;
                    }
                    x += dx, y += dy;
                }
                if (matching)
                {
                    max_score = std::max(max_score, pattern_score);
                }
            }
        }
        sum += max_score;
    }
    return sum;
}

void Chess::setPawn(const Integer &h, const Integer &w)
{
    check(h, w);
    if (status[h][w] != EMPTY)
    {
        printf("Error at: %s %d\n", __FILE__, __LINE__);
        getchar();
        exit(0);
    }
    const Real origin_white = calcScore(*this, h, w, WHITE_PAWN);
    const Real origin_black = calcScore(*this, h, w, BLACK_PAWN);
    status[h][w] = color;
    const Real fill_white = calcScore(*this, h, w, WHITE_PAWN);
    const Real fill_black = calcScore(*this, h, w, BLACK_PAWN);
    _score_white = _score_white + fill_white - origin_white;
    _score_black = _score_black + fill_black - origin_black;
    _color = opponent(_color);
    return;
}

Bool Chess::judge(const Byte &color) const
{
    const Integer DIRECTION[][2] = {{0, 1}, {1, 0}, {1, 1}, {-1, 1}};
    for (Integer h = 0; h < HEIGHT; h++)
    {
        for (Integer w = 0; w < WIDTH; w++)
        {
            if (status[h][w] != color)
            {
                continue;
            }
            for (Integer k = 0; k < (Integer)(sizeof(DIRECTION) / sizeof(DIRECTION[0])); k++)
            {
                Bool flag = WIN;
                Integer x = h, y = w;
                for (Integer i = 1; i < NUM_WIN; i++)
                {
                    x += DIRECTION[k][0], y += DIRECTION[k][1];
                    if (x < 0 || x >= HEIGHT || y < 0 || y >= WIDTH || status[x][y] != color)
                    {
                        flag = !WIN;
                        break;
                    }
                }
                if (flag == WIN)
                {
                    return WIN;
                }
            }
        }
    }
    return !WIN;
}

Byte Chess::judge() const
{
    if (judge(BLACK_PAWN) == WIN)
    {
        return BLACK_WIN;
    }
    if (judge(WHITE_PAWN) == WIN)
    {
        return WHITE_WIN;
    }
    for (Integer h = 0; h < HEIGHT; h++)
    {
        for (Integer w = 0; w < WIDTH; w++)
        {
            if (status[h][w] == EMPTY)
            {
                return UNFINISHED;
            }
        }
    }
    return EVEN;
}

Real Chess::white() const
{
    return score_white - score_black;
}

Real Chess::black() const
{
    return score_black - score_white;
}

Real Chess::score() const
{
    return score_black - score_white;
}

Chess &Chess::operator=(const Chess &chess)
{
    if (this != &chess)
    {
        memcpy(status, chess.status, sizeof(status));
    }
    return (*this);
}

Byte Chess::operator()(const Integer &h, const Integer &w) const
{
    check(h, w);
    return status[h][w];
}

Byte &Chess::operator()(const Integer &h, const Integer &w)
{
    check(h, w);
    return status[h][w];
}
