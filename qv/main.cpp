#include "qv.h"

int main(int argc, char *argv[])
{
    QV qv(argc, argv);
    while (qv.exec())
    {
        Int c = getch();
        if (c == 224 || c == 0)
        {
            c = (c + (getch() << 8));
        }
        qv.event(c);
    }
    return 0;
}
