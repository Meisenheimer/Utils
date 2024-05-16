#include "qv.h"

inline Int char2num(const Byte &c)
{
    if ('0' <= c && c <= '9')
    {
        return c - '0';
    }
    if ('a' <= c && c <= 'f')
    {
        return c - 'a' + 10;
    }
    if ('A' <= c && c <= 'F')
    {
        return c - 'A' + 10;
    }
    return -1;
}

inline Int string2num(const String &s)
{
    Int res;
    sscanf(s.c_str(), "%llX", &res);
    return res;
}

inline Bool isNum(const String &s)
{
    const Int n = s.length();
    for (Int i = 0; i < n; i++)
    {
        if (char2num(s[i]) == -1)
        {
            return false;
        }
    }
    return true;
}

inline List<String> split(const String &s)
{
    List<String> res;
    auto i = s.find_first_not_of(' ');
    while (i < s.length())
    {
        auto j = s.find_first_of(' ', i);
        if (j == s.npos)
        {
            res.push_back(s.substr(i));
            break;
        }
        res.push_back(s.substr(i, j - i));
        i = s.find_first_not_of(' ', j + 1);
    }
    return res;
}

void QV::displayInfo() const
{
    constexpr Int LEN_FILENAME = 40;
    constexpr Int POS_FILESIZE = 55;
    move(1, 1);
    fputs("Filename: ", stdout);
    if (filename.size() > LEN_FILENAME)
    {
        std::cout << filename.substr(0, LEN_FILENAME - 1) << "~";
    }
    else
    {
        std::cout << filename;
        for (Int i = filename.size(); i < LEN_FILENAME; i++)
        {
            putchar(' ');
        }
    }
    move(1, POS_FILESIZE);
    if (filesize >> 40)
    {
        printf("Filesize: %7.2lfTB\n", (double)filesize / (1 << 30));
    }
    else if (filesize >> 30)
    {
        printf("Filesize: %7.2lfGB\n", (double)filesize / (1 << 30));
    }
    else if (filesize >> 20)
    {
        printf("Filesize: %7.2lfMB\n", (double)filesize / (1 << 20));
    }
    else if (filesize >> 10)
    {
        printf("Filesize: %7.2lfKB\n", (double)filesize / (1 << 10));
    }
    else
    {
        printf("Filesize: %7lluB\n", filesize);
    }
    for (Int i = 0; i < 81; i++)
    {
        putchar('-');
    }
    move(3, 0);
    fputs((read_only ? "    Readonly" : "    Writable"), stdout);
    move(3, 18);
    color(Color::info);
    printf("0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F");
    move(20, 0);
    for (Int i = 0; i < 81; i++)
    {
        putchar('-');
    }
    move(22, 0);
    for (Int i = 0; i < 81; i++)
    {
        putchar('-');
    }
    putchar('\n');
    color();
    puts("Help: Tab - Mode, Ctrl+S - Save, ESC/Ctrl+C - Exit, F1 - HEX, F2 - CHAR, F3 - CMD");
    puts("Cmd: dump/dumpbin/dumptext [b c] - Save As (Byte, Bin, Text), move p - Set Offset");
    return;
}

void QV::display()
{
    page = (std::min<Int>(std::max<Int>(0, page), std::max<Int>(0, filesize - 1))) & MAX_PAGE_OFFSET;
    cursor = (std::min<Int>(std::max<Int>(0, cursor), MAX_CURSOR_OFFSET));
    Byte text[0x10][0x10] = {0};
    seek(fp, page, SEEK_SET);
    const Int n = std::min<Int>(0x10 * 0x10, filesize - page);
    fread(text, sizeof(Byte), n, fp);
    for (Int i = 0; i < 0x10; i++)
    {
        for (Int j = 0; j < 0x10; j++)
        {
            Int tmp = page + 0x10 * i + j;
            if (record.find(tmp) != record.end())
            {
                text[i][j] = record[tmp];
            }
        }
    }
    move(4, 0);
    for (Int i = 0; i < 0x10; i++)
    {
        color(Color::offset);
        printf("%016llX ", page + i * 0x10);
        for (Int j = 0; j < 0x10; j++)
        {
            Int tmp = 0x10 * i + j;
            color(tmp >= n ? Color::out : (record.find(page + tmp) != record.end() ? Color::modified : Color::text));
            printf("%02X ", text[i][j]);
        }
        for (Int j = 0; j < 0x10; j++)
        {
            Int tmp = 0x10 * i + j;
            color(tmp >= n ? Color::out : (record.find(page + tmp) != record.end() ? Color::modified : Color::text));
            putchar(isprint(text[i][j]) ? text[i][j] : '.');
        }
        putchar('\n');
    }

    Int i = cursor / 0x20;
    Int j = ((cursor / 2) & 0x0F);
    Byte c = (cursor & 1) ? text[i][j] & 0x0F : (text[i][j] & 0xF0) >> 4;

    move(4 + i, 18 + 3 * j + (mode == HEX ? (cursor & 1) : 0));
    color(mode == HEX ? Color::cursor : Color::show);
    printf("%X", mode == HEX ? c : text[i][j]);
    color();
    move(4 + i, 66 + j);
    color(mode == CHAR ? Color::cursor : Color::show);
    putchar(isprint(text[i][j]) ? text[i][j] : '.');
    color();

    move(21, 0);
    clearline();
    if (mode == COMMAND)
    {
        move(21, 0);
        clearline();
        color();
        fputs(cmd.c_str(), stdout);
        color(Color::cursor);
        putchar(' ');
        color();
    }
    return;
}

void QV::movePage(const Int &offset)
{
    page += (offset & MAX_PAGE_OFFSET);
    return;
}

void QV::moveCursor(const Int &offset)
{
    cursor += offset;
    if (cursor < 0 && page == 0)
    {
        cursor = 0;
    }
    else if (cursor > MAX_CURSOR_OFFSET && page == (filesize & MAX_PAGE_OFFSET))
    {
        cursor = MAX_CURSOR_OFFSET;
    }
    else
    {
        while (cursor < 0)
        {
            cursor += 0x20;
            page -= 0x10;
        }
        while (cursor > MAX_CURSOR_OFFSET)
        {
            cursor -= 0x20;
            page += 0x10;
        }
    }
    return;
}

void QV::moveTo(const Int &offset)
{
    cursor = (offset - page) * 2;
    if (cursor < 0 || cursor >= MAX_CURSOR_OFFSET)
    {
        page = offset & MAX_PAGE_OFFSET;
        cursor = (offset - page) * 2;
    }
    return;
}

void QV::save()
{
    for (auto it = record.begin(); it != record.end(); it++)
    {
        Byte c = it->second;
        seek(fp, it->first, SEEK_SET);
        fwrite(&c, sizeof(Byte), 1, fp);
    }
    record.clear();
    seek(fp, 0, SEEK_END);
    filesize = tell(fp);
    displayInfo();
    return;
}

void QV::open()
{
    if (read_only)
    {
        fp = fopen(filename.c_str(), "rb");
    }
    else
    {
        fp = fopen(filename.c_str(), "rb+");
        if (fp == nullptr)
        {
            fp = fopen(filename.c_str(), "rb");
            read_only = true;
        }
    }
    if (fp == nullptr)
    {
        std::cout << "Can't open " << filename << std::endl;
    }
    seek(fp, 0, SEEK_END);
    filesize = tell(fp);
    return;
}

void QV::run()
{
    List<String> tmp = split(cmd);
    if (tmp[0] == "exit")
    {
        quit = true;
        return;
    }
    Int begin = 0;
    Int end = filesize - 1;
    if (tmp.size() == 2 && isNum(tmp[1]))
    {
        begin = std::max(begin, string2num(tmp[1]));
    }
    if (tmp.size() == 3 && isNum(tmp[2]))
    {
        end = std::min(end, string2num(tmp[2]));
    }
    if (tmp[0] == "dump")
    {
        dump(begin, end);
    }
    else if (tmp[0] == "dumpbin")
    {
        dumpbin(begin, end);
    }
    else if (tmp[0] == "dumptext")
    {
        dumptext(begin, end);
    }
    else if (tmp[0] == "move")
    {
        moveTo(std::min<Int>(std::max<Int>(0, begin), filesize));
    }
    cmd = "";
    return;
}

void QV::setMode(const Int &m)
{
    mode = m;
    move(0, 78);
    fputs(mode == HEX ? " HEX" : (mode == CHAR ? "CHAR" : " CMD"), stdout);
    if (mode == CHAR)
    {
        moveCursor(-(cursor & 1));
    }
    return;
}

void QV::dump(const Int &begin, const Int &end)
{
    if (end < begin)
    {
        cmd = "Invalid command: " + cmd;
        return;
    }
    seek(fp, begin, SEEK_SET);
    FILE *fout = fopen((filename + ".dump").c_str(), "w");
    if (fout == nullptr)
    {
        cmd = String("Can't write" + filename + ".dump.");
    }
    for (Int i = begin; i <= end; i++)
    {
        Byte c;
        fread(&c, sizeof(Byte), 1, fp);
        if (record.find(i) != record.end())
        {
            c = record[i];
        }
        fprintf(fout, "%X ", c);
    }
    fclose(fout);
    return;
}

void QV::dumptext(const Int &begin, const Int &end)
{
    if (end < begin)
    {
        cmd = "Invalid command: " + cmd;
        return;
    }
    seek(fp, begin, SEEK_SET);
    FILE *fout = fopen((filename + ".dumptext").c_str(), "w");
    if (fout == nullptr)
    {
        cmd = String("Can't write" + filename + ".dumptext.");
    }
    for (Int i = begin; i <= end; i++)
    {
        Byte c;
        fread(&c, sizeof(Byte), 1, fp);
        if (record.find(i) != record.end())
        {
            c = record[i];
        }
        fprintf(fout, "%c", c);
    }
    fclose(fout);
    return;
}

void QV::dumpbin(const Int &begin, const Int &end)
{
    if (end < begin)
    {
        cmd = "Invalid command: " + cmd;
        return;
    }
    seek(fp, begin, SEEK_SET);
    FILE *fout = fopen((filename + ".dumpbin").c_str(), "wb");
    if (fout == nullptr)
    {
        cmd = String("Can't write" + filename + ".dumpbin.");
    }
    for (Int i = begin; i <= end; i++)
    {
        Byte c;
        fread(&c, sizeof(Byte), 1, fp);
        if (record.find(i) != record.end())
        {
            c = record[i];
        }
        fwrite(&c, sizeof(Byte), 1, fout);
    }
    fclose(fout);
    return;
}

void QV::event(const Int &key)
{
    switch (key)
    {
    case 15104: // F1
        setMode(HEX);
        break;
    case 15360: // F2
        setMode(CHAR);
        break;
    case 15616: // F3
        setMode(COMMAND);
        break;
    case 8: // backspace
        if (mode == COMMAND && cmd.length() > 0)
        {
            cmd = cmd.substr(0, cmd.length() - 1);
        }
        break;
    case 9: // tab
        setMode((mode + 1) % MODE_NUM);
        break;
    case 13: // enter
        if (mode == COMMAND)
        {
            run();
        }
        if (mode == HEX || mode == CHAR)
        {
            moveCursor(0x20);
        }
        break;
    case 19: // ctrl s
        save();
        break;
    case 3:  // ctrl c
    case 27: // esc
        quit = true;
        break;
    case 18656: // up
        moveCursor(-0x20);
        break;
    case 20704: // down
        moveCursor(0x20);
        break;
    case 19424: // left
        moveCursor(-(mode == HEX ? 0x1 : 0x2));
        break;
    case 19936: // right
        moveCursor((mode == HEX ? 0x1 : 0x2));
        break;
    case 18912: // page up
        movePage(-0x100);
        break;
    case 20960: // page down
        movePage(0x100);
        break;
    case 18400: // home
        moveTo(0);
        break;
    case 20448: // end
        moveTo(filesize - 1);
        break;
    case 36320: // ctrl up
        movePage(-0x10);
        break;
    case 37344: // ctrl down
        movePage(0x10);
        break;
    default:
        if (mode == HEX && char2num(key) != -1)
        {
            Int pos = page + (cursor / 2);
            Byte c = 0;
            if (record.find(pos) != record.end())
            {
                c = record[pos];
            }
            else if (pos < filesize)
            {
                seek(fp, pos, SEEK_SET);
                fread(&c, sizeof(Byte), 1, fp);
            }
            if (cursor & 1)
            {
                c = ((c & 0xF0) | char2num(key));
            }
            else
            {
                c = ((char2num(key) << 4) | (c & 0x0F));
            }
            moveCursor(0x1);
            record[pos] = c;
        }
        else if (mode == CHAR && isprint(key))
        {
            Int pos = page + (cursor / 2);
            moveCursor(0x2);
            record[pos] = key;
        }
        else if (mode == COMMAND && isprint(key)) // enter
        {
            cmd += key;
        }
        break;
    }
    display();
    return;
}

Bool QV::exec() const
{
    return !quit;
}

QV::QV(int argc, char *argv[])
{
    quit = false;
    if (argc < 2)
    {
        printf("Please input the file name.\n");
        exit(0);
    }
    filename = argv[1];
    read_only = false;
    for (Int i = 2; i < argc; i++)
    {
        if (0 == strcmp(argv[i], "-r"))
        {
            read_only = true;
        }
    }
    open();
    clear();
    showCursor(false);
    displayInfo();
    page = 0;
    cursor = 0;
    setMode(HEX);
    display();
}

QV::~QV()
{
    showCursor(true);
    clear();
    if (fp != nullptr)
    {
        fclose(fp);
    }
}