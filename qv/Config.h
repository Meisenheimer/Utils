#ifndef MW_CONFIG_H
#define MW_CONFIG_H

#include <conio.h>
#include <string>
#include <map>
#include <cstring>
#include <vector>
#include <iostream>
#include <set>
#include <functional>

using Bool = bool;
using Byte = unsigned char;
using Int = long long int;
using Real = long double;
using String = std::string;
template <typename Data>
using List = std::vector<Data>;
template <typename Data>
using Set = std::set<Data>;
template <typename Key, typename Data>
using Map = std::map<Key, Data>;
template <typename First, typename Second>
using Pair = std::pair<First, Second>;

inline auto seek = fseeko64;
inline auto tell = ftello64;

constexpr Int MAX_PAGE_OFFSET = 0xFFFFFFFFFFFFFFF0;
constexpr Int MAX_CURSOR_OFFSET = 0x1FF;

enum Color
{
    info,
    offset,
    text,
    modified,
    cursor,
    show,
    out,
    none
};

void move(const Byte &x, const Byte &y);
void color(const Color &type);
void clear();
void clearline();
void showCursor(const Bool flag);

inline void move(const Byte &i, const Byte &j)
{
    printf("%c[%d;%df", 0x1B, i, j);
}

inline void color(const Color &type = Color::none)
{
    switch (type)
    {
    case info:
        fputs("\033[38;2;153;153;153m", stdout);
        break;
    case offset:
        fputs("\033[38;2;153;153;153m", stdout);
        break;
    case text:
        fputs("\033[38;2;170;170;170m", stdout);
        break;
    case modified:
        fputs("\033[38;2;255;85;85m", stdout);
        break;
    case cursor:
        fputs("\033[1;4m", stdout);
        fputs("\033[38;2;255;255;255m", stdout);
        fputs("\033[48;2;85;85;85m", stdout);
        break;
    case show:
        fputs("\033[1;4m", stdout);
        fputs("\033[38;2;255;255;255m", stdout);
        break;
    case out:
        fputs("\033[38;2;102;102;102m", stdout);
        break;
    default:
        fputs("\033[0m", stdout);
        break;
    }
    return;
}

inline void clear()
{
#ifdef _WIN32
    system("chcp 437");
    system("cls");
#elif __linux__
    system("clear");
#endif
    fputs("\033[0m", stdout);
    return;
}

inline void clearline()
{
    fputs("\33[2K\r", stdout);
    fputs("\033[0m", stdout);
    return;
}

inline void showCursor(const Bool flag)
{
    if (flag)
    {
        fputs("\033[?25h", stdout);
    }
    else
    {
        fputs("\033[?25l", stdout);
    }
    return;
}

#endif