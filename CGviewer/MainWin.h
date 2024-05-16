#ifndef MAINWIN_H
#define MAINWIN_H

#include "Config.h"

class MainWin : public QWidget
{
    Q_OBJECT

private:
    Int scale_mode;
    QVector<Real> scale;
    QImage figure;
    QPoint figure_pos;
    QSize figure_size;

    QPointF click_point;

private:
    QLabel *lbl_title;
    QLabel *btn_menu;
    Bool btn_menu_clicked;
    QLabel *lbl_figure;
    Bool lbl_figure_clicked;

private:
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);

    void drawUI();

    void openMenu();
    void changeScale(const QPoint &p);

public:
    MainWin(QWidget *parent = nullptr);
    ~MainWin();
};

#endif // MAINWIN_H
