#ifndef MAINWIN_H
#define MAINWIN_H

#include "Config.h"
#include "Scene.h"

class MainWin : public QWidget
{
    Q_OBJECT

public:
    MainWin(QWidget *parent = nullptr);
    ~MainWin();

private slots:
    void loadJson();
    void saveJson();
    void saveScene();
    void setIndex();
    void setIndex(const Int &n);
    void displayUI();
    void displayScene();
    void moveUp();
    void moveDown();
    void addScene();
    void delScene();

private:
    void loadCompleter();

    void resizeEvent(QResizeEvent *event);

private:
    Int index;
    QString filename;
    QList<Scene> scene;
    QList<QShortcut *> shortcut;

    QCompleter *id_completer;
    QCompleter *motion_completer;
    QCompleter *face_completer;
    QCompleter *music_completer;
    QCompleter *background_completer;

    QLabel *path_label;
    QLineEdit *path;
    QPushButton *load;
    QPushButton *save;

    QLabel *list_label;
    QListWidget *list;
    QPushButton *move_up;
    QPushButton *move_down;
    QPushButton *add;
    QPushButton *del;

    QLabel *id_label[NUM_CHARACTER];
    QLabel *motion_label[NUM_CHARACTER];
    QLabel *face_label[NUM_CHARACTER];
    QLabel *title_label;
    QLabel *text_label;
    QLabel *speaker_label;
    QLabel *music_label;
    QLabel *background_label;

    QLineEdit *id[NUM_CHARACTER];
    QLineEdit *motion[NUM_CHARACTER];
    QLineEdit *face[NUM_CHARACTER];
    QLineEdit *title;
    QTextEdit *text;
    QLineEdit *speaker;
    QLineEdit *music;
    QLineEdit *background;

    QLineEdit *status;
};

#endif // MAINWIN_H
