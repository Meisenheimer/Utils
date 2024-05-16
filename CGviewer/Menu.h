#ifndef MENU_H
#define MENU_H

#include "Config.h"

class Menu : public QDialog
{
    Q_OBJECT

private:
    QString current_path;

private:
    QLabel *lbl_title;
    QLabel *btn_menu;
    Bool btn_menu_clicked;
    QLabel *lbl_path;
    QListWidget *lw_list;

public:
    explicit Menu(QWidget *parent);
    ~Menu();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);

    void drawUI();

    void closeMenu();

    void setPath(const QString &path);
};

#endif // MENU_H
