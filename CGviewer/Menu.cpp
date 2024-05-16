#include "Menu.h"

Menu::Menu(QWidget *parent) : QDialog(parent)
{
    setFixedSize(WIDTH, HEIGHT);
    lbl_title = new QLabel(this);
    lbl_title->setText("CG Viewer - Menu");
    lbl_title->setStyleSheet("QLabel{background-color:rgba(176,31,36,1);color:white}");

    btn_menu = new QLabel(this);
    btn_menu->setStyleSheet("QLabel{background-color:rgba(176,31,36,1);color:white};");

    lbl_path = new QLabel(this);

    lw_list = new QListWidget(this);

    current_path = BASE_PATH;

    drawUI();
}

Menu::~Menu()
{
    delete lbl_title;
    delete btn_menu;
    delete lw_list;
}

void Menu::mousePressEvent(QMouseEvent *event)
{
    QPointF pos = event->position();
    btn_menu_clicked = ((UInt)pos.x() <= ICON_SIZE && (UInt)pos.y() <= ICON_SIZE);
    return;
}

void Menu::mouseMoveEvent(QMouseEvent *event)
{
    QPointF pos = event->position();
    if (btn_menu_clicked && ((UInt)pos.x() > ICON_SIZE || (UInt)pos.y() > ICON_SIZE))
    {
        btn_menu_clicked = false;
    }
    return;
}

void Menu::mouseReleaseEvent(QMouseEvent *event)
{
    QPointF pos = event->position();
    if (btn_menu_clicked && ((UInt)pos.x() > ICON_SIZE || (UInt)pos.y() > ICON_SIZE))
    {
        btn_menu_clicked = false;
    }
    if (btn_menu_clicked)
    {
        closeMenu();
    }
    return;
}

void Menu::resizeEvent(QResizeEvent *event)
{
    drawUI();
    return;
}

void Menu::drawUI()
{
    QRect screen = QGuiApplication::primaryScreen()->geometry();
    HEIGHT = screen.height();
    WIDTH = screen.width();
    ICON_SIZE = qMax(HEIGHT / 16, WIDTH / 16);

    lbl_title->move(ICON_SIZE, 0);
    lbl_title->setFont(QFont("Microsoft YaHei", 0.8 * ICON_SIZE));
    lbl_title->setFixedSize(WIDTH - ICON_SIZE, ICON_SIZE);
    lbl_title->show();

    btn_menu->setPixmap(QPixmap(":/icon/icon/gear_white.svg").scaled(0.8 * ICON_SIZE, 0.8 * ICON_SIZE));
    btn_menu->move(0, 0);
    btn_menu->setFixedSize(ICON_SIZE, ICON_SIZE);
    btn_menu->show();

    lbl_path->move(0, ICON_SIZE);
    lbl_path->setFont(QFont("Microsoft YaHei", 0.3 * ICON_SIZE));
    lbl_path->setFixedSize(WIDTH, ICON_SIZE / 2);
    lbl_path->show();

    lw_list->move(0, 3 * ICON_SIZE / 2);
    lw_list->setFixedSize(WIDTH, HEIGHT - 3 * ICON_SIZE / 2);
    lw_list->show();

    setPath(current_path);

    return;
}

void Menu::closeMenu()
{
    close();
    return;
}

void Menu::setPath(const QString &path)
{
    lbl_path->setText(current_path);
    const QVector<QString> list = listDir(path);
    lw_list->clear();
    lw_list->addItem(QString::number(list.size()));
    for (Int i = 0; i < list.size(); i++)
    {
        lw_list->addItem(list[i]);
    }
    return;
}