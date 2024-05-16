#ifndef MW_QV_H
#define MW_QV_H

#include "Config.h"

constexpr Int MODE_NUM = 3;
constexpr Int HEX = 0;
constexpr Int CHAR = 1;
constexpr Int COMMAND = 2;

class QV
{
private:
    void displayInfo() const;
    void display();

    void movePage(const Int &offset);
    void moveCursor(const Int &offset);
    void moveTo(const Int &offset);
    void save();
    void open();
    void run();
    void setMode(const Int &m);

    void dump(const Int &begin, const Int &end);
    void dumptext(const Int &begin, const Int &end);
    void dumpbin(const Int &begin, const Int &end);

public:
    void event(const Int &key);
    Bool exec() const;

    explicit QV(int argc, char *argv[]);
    ~QV();

private:
    Bool quit;
    Bool read_only;
    Int mode;
    Int page;
    Int cursor;
    Int filesize;
    String filename;
    String cmd;
    FILE *fp;

    Map<Int, Byte> record;
};

#endif // QV_H
