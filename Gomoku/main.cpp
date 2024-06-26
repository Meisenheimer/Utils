#include <graphics.h>

#include "Chess.h"
#include "Strategy.h"

void display(const Chess &chess);

int main()
{
    Integer win_cnt[3] = {0};
    initgraph((WIDTH + 2) * GRID_SIZE, (HEIGHT + 2) * GRID_SIZE, INIT_RENDERMANUAL);
    show_console();
    setbkcolor_f(EGERGB(240, 155, 90));
    settextcolor(EGERGB(0, 0, 0));
    setlinecolor(EGERGB(128, 128, 128));
    while (true)
    {
        system("cls");
        Chess chess;
        Byte res;
        while (true)
        {
            display(chess);
            std::pair<Integer, Integer> pos;
            auto t0 = std::chrono::steady_clock::now();
            if (chess.color == BLACK_PAWN)
            {
                pos = blackStrategy(chess);
            }
            if (chess.color == WHITE_PAWN)
            {
                pos = whiteStrategy(chess);
            }
            auto t1 = std::chrono::steady_clock::now();
            printf("-%s %ld %ld ", COLOR_NAME[chess.color], pos.first, pos.second);
            printf("time = %lld\n", std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count());
            chess.setPawn(pos.first, pos.second);
            res = chess.judge();
            if (res == BLACK_WIN || res == WHITE_WIN)
            {
                win_cnt[res]++;
                break;
            }
        }
        display(chess);
        printf("%s Win.\n", COLOR_NAME[res]);
        getchar();
    }
    closegraph();
    return 0;
}

inline void display(const Chess &chess)
{
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    cleardevice();
    for (Integer h = 0; h <= HEIGHT; h++)
    {
        line(GRID_SIZE, (h + 1) * GRID_SIZE, (WIDTH + 1) * GRID_SIZE, (h + 1) * GRID_SIZE);
    }
    for (Integer w = 0; w <= WIDTH; w++)
    {
        line((w + 1) * GRID_SIZE, GRID_SIZE, (w + 1) * GRID_SIZE, (HEIGHT + 1) * GRID_SIZE);
    }
    for (Integer h = 0; h < HEIGHT; h++)
    {
        outtextrect(0, (h + 1) * GRID_SIZE, GRID_SIZE, GRID_SIZE, (LPCTSTR)std::to_string(h).c_str());
        outtextrect((WIDTH + 1) * GRID_SIZE, (h + 1) * GRID_SIZE, GRID_SIZE, GRID_SIZE, (LPCTSTR)std::to_string(h).c_str());
    }
    for (Integer w = 0; w < WIDTH; w++)
    {
        outtextrect((w + 1) * GRID_SIZE, 0, GRID_SIZE, GRID_SIZE, (LPCTSTR)std::to_string(w).c_str());
        outtextrect((w + 1) * GRID_SIZE, (WIDTH + 1) * GRID_SIZE, GRID_SIZE, GRID_SIZE, (LPCTSTR)std::to_string(w).c_str());
    }
    for (Integer h = 0; h < HEIGHT; h++)
    {
        for (Integer w = 0; w < WIDTH; w++)
        {
            if (chess(h, w) == BLACK_PAWN)
            {
                setfillcolor(EGERGB(51, 51, 51));
                solidcircle((2 * w + 3) * GRID_SIZE / 2, (2 * h + 3) * GRID_SIZE / 2, (Integer)(0.4 * GRID_SIZE));
            }
            if (chess(h, w) == WHITE_PAWN)
            {
                setfillcolor(EGERGB(204, 204, 204));
                solidcircle((2 * w + 3) * GRID_SIZE / 2, (2 * h + 3) * GRID_SIZE / 2, (Integer)(0.4 * GRID_SIZE));
            }
        }
    }
    delay_ms(0);
    return;
}
