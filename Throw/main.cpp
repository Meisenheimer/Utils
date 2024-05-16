#include <iostream>
#include <vector>
#include <set>

#include <graphics.h>

#include "Curve.h"

Point start = makePoint(-5, -3);
Point target = makePoint(5, -3);
std::vector<Curve> curves;
std::vector<Point> cubes;
std::vector<Point> point = {makePoint(0, -3), makePoint(0, 10)};
std::vector<Line> lines;
std::vector<Real> list;

void display();
void calc();
void addCube(const Real &x, const Real &y);
void print();

int main()
{
    Bool flag = true;
    initgraph(WIDTH, HEIGHT, SHOWCONSOLE);
    setorigin(WIDTH / 2, HEIGHT / 2);
    setaspectratio(1, -1);
    setbkcolor(RGB(51, 51, 51));
    settextcolor(RGB(255, 255, 255));
    setfillcolor(RGB(204, 204, 204));
    while (flag)
    {
        char c;
        Real x, y;
        calc();
        display();
        scanf("%c%lf%lf", &c, &x, &y);
        if (c == 'q')
        {
            flag = false;
        }
        else if (c == 'a')
        {
            addCube(x, y);
        }
        else if (c == 's')
        {
            start = makePoint(x, y);
        }
        else if (c == 't')
        {
            target = makePoint(x, y);
        }
        point[0] = (start + target) / 2;
        point[1] = (start + target) / 2;
        point[1](1) = 9;
    }
    closegraph();
    return 0;
}

inline void display()
{
    print();
    BeginBatchDraw();
    cleardevice();

    setlinecolor(RGB(85, 85, 85));
    setlinestyle(PS_SOLID, 1);
    for (Integer i = -std::max(HEIGHT, WIDTH) / 2; i <= std::max(HEIGHT, WIDTH) / 2; i += SCALE)
    {
        line(-WIDTH, i, WIDTH, i);
        line(i, HEIGHT, i, -HEIGHT);
    }

    setlinecolor(RGB(170, 170, 170));
    setlinestyle(PS_SOLID, 2);
    line(-WIDTH, 0, WIDTH, 0);
    line(0, HEIGHT, 0, -HEIGHT);

    setlinecolor(RGB(0, 0, 255));
    setlinestyle(PS_SOLID, 2);
    fillcircle(start(0) * SCALE, start(1) * SCALE, BALL_RADIUS * SCALE);
    fillcircle(target(0) * SCALE, target(1) * SCALE, DAMAGE_RADIUS * SCALE);

    setlinecolor(RGB(0, 255, 0));
    setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 2);
    for (Integer i = 0; i < (Integer)curves.size(); i++)
    {
        Real step = ((target(0) - start(0)) / (Real)STEP);
        Real x1 = start(0);
        Real x2 = x1 + step;
        for (Integer k = 0; k < STEP; k++)
        {
            line((Integer)SCALE * x1, (Integer)SCALE * curves[i](x1), (Integer)SCALE * x2, (Integer)SCALE * curves[i](x2));
            x1 = x2, x2 += step;
        }
    }
    setlinecolor(RGB(255, 0, 0));
    setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 2);
    for (Integer i = 0; i < (Integer)point.size(); i++)
    {
        Curve c(point[i], start, target);
        if (c.a > 0)
        {
            continue;
        }
        Real step = ((target(0) - start(0)) / (Real)STEP);
        Real x1 = start(0);
        Real x2 = x1 + step;
        for (Integer k = 0; k < STEP; k++)
        {
            line((Integer)SCALE * x1, (Integer)SCALE * c(x1), (Integer)SCALE * x2, (Integer)SCALE * c(x2));
            x1 = x2, x2 += step;
        }
    }
    for (Integer i = 0; i < (Integer)cubes.size(); i++)
    {
        Integer x = (Integer)(SCALE * cubes[i](0));
        Integer y = (Integer)(SCALE * cubes[i](1));
        solidrectangle(x - SCALE / 2, y + SCALE / 2, x + SCALE / 2, y - SCALE / 2);
    }
    EndBatchDraw();
    return;
}

inline void calc()
{
    curves.clear();
    list.clear();
    for (Integer i = 0; i < (Integer)point.size(); i++)
    {
        list.push_back(std::min(Curve(point[i], start, target).a, 0.0));
    }
    std::sort(list.begin(), list.end());
    list.erase(std::unique(list.begin(), list.end()), list.end());
    for (Integer i = 1; i < (Integer)list.size(); i++)
    {
        Curve c(((list.at(i - 1) + list.at(i)) / 2.0), start, target);
        Bool flag = true;
        for (Integer i = 0; i < (Integer)lines.size(); i++)
        {
            if (lines[i].intersect(c.a, c.b, c.c))
            {
                flag = false;
                break;
            }
        }
        if (flag)
        {
            curves.push_back(c);
        }
    }
    return;
}

inline void addCube(const Real &x, const Real &y)
{
    cubes.push_back(makePoint(x, y));
    std::vector<Point> p = {makePoint(x - 0.5, y + 0.5),
                            makePoint(x + 0.5, y + 0.5),
                            makePoint(x + 0.5, y - 0.5),
                            makePoint(x - 0.5, y - 0.5)};
    point.insert(point.end(), p.begin(), p.end());
    lines.push_back(Line(p[0], p[1]));
    lines.push_back(Line(p[1], p[2]));
    lines.push_back(Line(p[2], p[3]));
    lines.push_back(Line(p[3], p[0]));
    lines.push_back(Line(p[0], p[2]));
    lines.push_back(Line(p[1], p[3]));
    return;
}

inline void print()
{
    printf("-----print begin-----\n");
    std::cout << "num of a: " << list.size() << std::endl;
    for (Integer i = 0; i < (Integer)list.size(); i++)
    {
        std::cout << list[i] << std::endl;
    }
    std::cout << "num of curve: " << curves.size() << std::endl;
    for (Integer i = 0; i < (Integer)curves.size(); i++)
    {
        printf("%lf %lf %lf\n", curves[i].a, curves[i].b, curves[i].c);
    }
    std::cout << "num of point: " << point.size() << std::endl;
    for (Integer i = 0; i < (Integer)point.size(); i++)
    {
        Curve c(point[i], start, target);
        std::cout << point[i].transpose() << " " << c.a << " " << c.b << " " << c.c << std::endl;
    }
    std::cout << "num of lines: " << lines.size() << std::endl;
    for (Integer i = 0; i < (Integer)lines.size(); i++)
    {
        std::cout << lines[i].p1.transpose() << " --- ";
        std::cout << lines[i].p2.transpose() << std::endl;
    }
    printf("-----print   end-----\n");
}