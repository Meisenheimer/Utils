#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

// #include <QDialog>
#include <QFileDialog>
#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QCompleter>

#include <QString>
#include <QList>
#include <QShortcut>

#include <QFont>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonParseError>

using Int = int64_t;

const QFont FONT = QFont("Microsoft YaHei", 12);

constexpr Int MIN_WIDTH = 1280;
constexpr Int MIN_HEIGHT = 720;

constexpr Int NUM_CHARACTER = 3;

#endif // CONFIG_H
