/**
 * @file encode.cpp
 * @brief The main file of the program, which can encode/decode the given file.
 * @author Wang Zeyu (zeyuwang@zuaa.zju.edu.cn)
 * @date 2024-04-17
 */

#include <conio.h>
#include <iostream>
#include <string>
#include <windows.h>
#include <queue>
#include <vector>

#include <QFileInfo>
#include <QDir>
#include <QQueue>
#include <QTextStream>

using Bool = bool;
using Byte = unsigned char;
using Int = long long int;
using String = std::string;

QTextStream cin(stdin, QIODevice::ReadOnly);
QTextStream cout(stdout, QIODevice::WriteOnly);

void encode(const QString &filename, const QString password);

int main(int argc, char *argv[])
{
    // the argc should be 1 and argv[1] is the given filename.
    SetConsoleOutputCP(CP_UTF8);
    if (argc != 2)
    {
        printf("Error At: %s %d\n", __FILE__, __LINE__);
        exit(0);
    }
    QString path = QString(argv[1]);
    QString password;

    // Input the secret key.
    cout << "Key: ";
    cout.flush();
    cin >> password;
    cout << "Key: " << password << Qt::endl;
    cout.flush();

    // Encode/Decode all the files in the directory and subdirectories.
    QQueue<QString> list;
    list.enqueue(path);
    while (!list.isEmpty())
    {
        path = list.dequeue();
        QFileInfo info(path);
        if (info.isDir())
        {
            // If the given filename is a directory, load all the files and subdirectories.
            QDir dir(path);
            QStringList tmp = dir.entryList(QDir::Files | QDir::NoDotAndDotDot | QDir::Dirs);
            for (Int i = 0; i < tmp.size(); i++)
            {
                list.enqueue(QDir::cleanPath(path + QDir::separator() + tmp[i]));
            }
        }
        else if (info.isFile() && !info.isSymLink())
        {
            // If the given filename is a file, encode/decode it.
            cout << path << Qt::endl;
            cout.flush();
            int c = getch();
            if (c != '\r')
            {
                exit(0);
            }
            encode(path, password);
        }
        else
        {
            cout << "Alert: " << path << " not exists" << Qt::endl;
        }
    }
    return 0;
}

/**
 * @brief Given the filename, the function will: 1. read the file; 2. encode/decode the content; 3. write the file.
 * @param  filename         The file that needs to be encoded/decoded.
 * @param  password         The secret key given by the user.
 */
inline void encode(const QString &filename, const QString password)
{
    const String key = password.toStdString();
    const Int len = key.length();

    // Read the file.
    FILE *fp = fopen(filename.toStdString().c_str(), "rb");
    if (fp == nullptr)
    {
        cout << "Alert: Can't open " << filename << Qt::endl;
        return;
    }
    fseeko64(fp, 0, SEEK_END);
    const Int size = ftello64(fp);

    // Alert when can't read.
    Byte *p = new Byte[size];
    fseeko64(fp, 0, SEEK_SET);
    Int tmp = fread(p, sizeof(Byte), size, fp);
    if (tmp != size)
    {
        cout << "Alert: Can't read " << filename << Qt::endl;
        return;
    }
    fclose(fp);

    // Encode/Decode.
    Byte k = 0;
    for (Int i = 0; i < size; i++, k += ((Byte)key[i % len] + i))
    {
        p[i] = p[i] ^ k;
    }

    // Alert when can't write.
    fp = fopen(filename.toStdString().c_str(), "wb");
    if (fp == nullptr)
    {
        cout << "Alert: Can't open " << filename << Qt::endl;
        return;
    }
    // Write back to the file.
    tmp = fwrite(p, sizeof(Byte), size, fp);
    if (tmp != size)
    {
        cout << "Alert: Can't write " << filename << Qt::endl;
        return;
    }
    fclose(fp);

    delete[] p;
    return;
}
