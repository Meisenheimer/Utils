#ifndef CONFIG_H
#define CONFIG_H

#include <QPalette>
#include <QDir>
#include <QFileInfo>
#include <QSet>
#include <QFileDialog>
#include <QGuiApplication>
#include <QMouseEvent>
#include <QMessageBox>
#include <QScreen>
#include <QFont>
#include <QLabel>
#include <QWidget>
#include <QDialog>
#include <QListWidget>
#include <QQueue>

using Bool = bool;
using Byte = unsigned char;
using Int = long long int;
using UInt = unsigned long long int;
using Real = double;

struct Figure;

inline Int HEIGHT;
inline Int WIDTH;
inline Int ICON_SIZE;
inline QSet<QString> FIGURE;
inline QString BASE_PATH;

QVector<QString> listDir(const QString &path);
QImage loadFigure();

struct Figure
{
    Real layer;
    QString name;
    QString path;

    Figure(const QString &path);
};

#endif // CONFIG_H
