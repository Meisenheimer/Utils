#include "MainWin.h"
#include "Menu.h"

void MainWin::mouseDoubleClickEvent(QMouseEvent *event)
{
    QPoint pos = event->position().toPoint();
    auto tmp = QRect(figure_pos, figure_size);
    qDebug() << QString("Double Clicked (%1, %2), (%3 %4 %5 %6)\n").arg(pos.x()).arg(pos.y()).arg(tmp.left()).arg(tmp.top()).arg(tmp.right()).arg(tmp.bottom());
    if (QRect(figure_pos, figure_size).contains(pos))
    {
        changeScale(pos);
    }
    return;
}

void MainWin::mousePressEvent(QMouseEvent *event)
{
    click_point = event->position();
    btn_menu_clicked = QRect(btn_menu->pos(), btn_menu->size()).contains(click_point.toPoint());
    lbl_figure_clicked = QRect(lbl_figure->pos(), lbl_figure->size()).contains(click_point.toPoint());
    return;
}

void MainWin::mouseMoveEvent(QMouseEvent *event)
{
    auto new_click_point = event->position();
    if (btn_menu_clicked && !QRect(btn_menu->pos(), btn_menu->size()).contains(new_click_point.toPoint()))
    {
        btn_menu_clicked = false;
    }
    if (lbl_figure_clicked)
    {
        figure_pos += (new_click_point - click_point).toPoint();
        lbl_figure->move(figure_pos);
    }
    click_point = new_click_point;
    return;
}

void MainWin::mouseReleaseEvent(QMouseEvent *event)
{
    if (btn_menu_clicked && !QRect(btn_menu->pos(), btn_menu->size()).contains(event->position().toPoint()))
    {
        btn_menu_clicked = false;
    }
    if (btn_menu_clicked)
    {
        openMenu();
    }
    btn_menu_clicked = false;
    lbl_figure_clicked = false;
    return;
}

void MainWin::resizeEvent(QResizeEvent *event)
{
    drawUI();
    return;
}

void MainWin::drawUI()
{
    QRect screen = QGuiApplication::primaryScreen()->geometry();
    HEIGHT = screen.height();
    WIDTH = screen.width();
    ICON_SIZE = qMax(HEIGHT / 16, WIDTH / 16);

    lbl_title->move(ICON_SIZE, 0);
    lbl_title->setFont(QFont("Microsoft YaHei", 0.8 * ICON_SIZE));
    lbl_title->setFixedSize(WIDTH - ICON_SIZE, ICON_SIZE);

    btn_menu->setPixmap(QPixmap(":/icon/icon/gear_white.svg").scaled(0.8 * ICON_SIZE, 0.8 * ICON_SIZE));
    btn_menu->move(0, 0);
    btn_menu->setFixedSize(ICON_SIZE, ICON_SIZE);

    figure = QImage(QSize(0, 0), QImage::Format_RGBA32FPx4);
    for (auto it = FIGURE.begin(); it != FIGURE.end(); it++)
    {
        QImage tmp(*it);
        figure = tmp;
    }
    scale_mode = 0;
    figure_size = figure.size();
    figure_pos = QPoint(WIDTH / 2 - figure_size.width() / 2, (HEIGHT + ICON_SIZE) / 2 - figure_size.height() / 2);
    qDebug() << QString("Old Pos (%1 %2)\n").arg(figure_pos.x()).arg(figure_pos.x());
    lbl_figure->setPixmap(QPixmap::fromImage(figure));
    lbl_figure->move(figure_pos);
    lbl_figure->setFixedSize(figure_size);

    return;
}

void MainWin::openMenu()
{
    // BASE_PATH = QFileDialog::getExistingDirectory(this, tr("选择目录"), BASE_PATH);
    BASE_PATH = QFileDialog::getOpenFileName(this, tr("选择文件"), BASE_PATH);
    FIGURE.clear();
    FIGURE.insert(BASE_PATH);
    // Menu menu(this);
    // menu.show();
    // menu.exec();
    drawUI();
    return;
}

void MainWin::changeScale(const QPoint &p)
{
    Int old_scale = scale[scale_mode];
    figure_size /= old_scale;

    scale_mode = (scale_mode + 1) % (Int)scale.size();
    qDebug() << QString("Set Scale To %1\n").arg(scale[scale_mode]);

    figure_size *= scale[scale_mode];
    qDebug() << QString("P (%1 %2)\n").arg(p.x()).arg(p.x());
    qDebug() << QString("Old Pos (%1 %2)\n").arg(figure_pos.x()).arg(figure_pos.x());
    figure_pos = p + scale[scale_mode] * (figure_pos - p) / old_scale;
    qDebug() << QString("Old Pos (%1 %2)\n").arg(figure_pos.x()).arg(figure_pos.x());

    lbl_figure->setFixedSize(figure_size);
    lbl_figure->setPixmap(QPixmap::fromImage(figure.scaled(figure_size)));
    lbl_figure->move(figure_pos);
    return;
}

MainWin::MainWin(QWidget *parent) : QWidget(parent)
{
    scale.push_back(1);
    scale.push_back(2);
    scale.push_back(4);
    scale.push_back(8);
    lbl_title = new QLabel(this);
    lbl_title->setText("CG Viewer");
    lbl_title->setStyleSheet("QLabel{background-color:rgba(0,63,136,1);color:white}");

    btn_menu = new QLabel(this);
    btn_menu->setStyleSheet("QLabel{background-color:rgba(0,63,136,1);color:white};");

    lbl_figure = new QLabel(this);
    lbl_figure->setStyleSheet("QLabel{background-color:rgba(255,255,255,1);color:white};");

    QPalette pal(this->palette());
    pal.setColor(QPalette::Window, Qt::black);
    setAutoFillBackground(true);
    setPalette(pal);

    lbl_figure->show();
    lbl_title->show();
    btn_menu->show();

    lbl_title->raise();
    btn_menu->raise();

    drawUI();
}

MainWin::~MainWin()
{
    delete lbl_title;
    delete btn_menu;
    delete lbl_figure;
}
