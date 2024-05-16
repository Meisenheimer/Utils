#include "MainWin.h"

MainWin::MainWin(QWidget *parent)
    : QWidget(parent)
{
    path_label = new QLabel(this);
    path = new QLineEdit(this);
    load = new QPushButton(this);
    save = new QPushButton(this);
    list_label = new QLabel(this);
    list = new QListWidget(this);
    move_up = new QPushButton(this);
    move_down = new QPushButton(this);
    add = new QPushButton(this);
    del = new QPushButton(this);
    for (Int i = 0; i < NUM_CHARACTER; i++)
    {
        id[i] = new QLineEdit(this);
        motion[i] = new QLineEdit(this);
        face[i] = new QLineEdit(this);
        id_label[i] = new QLabel(this);
        motion_label[i] = new QLabel(this);
        face_label[i] = new QLabel(this);
    }
    title = new QLineEdit(this);
    text = new QTextEdit(this);
    speaker = new QLineEdit(this);
    music = new QLineEdit(this);
    background = new QLineEdit(this);
    title_label = new QLabel(this);
    text_label = new QLabel(this);
    speaker_label = new QLabel(this);
    music_label = new QLabel(this);
    background_label = new QLabel(this);
    status = new QLineEdit(this);

    path_label->setFont(FONT);
    path->setFont(FONT);
    load->setFont(FONT);
    save->setFont(FONT);
    list_label->setFont(FONT);
    list->setFont(FONT);
    move_up->setFont(FONT);
    move_down->setFont(FONT);
    add->setFont(FONT);
    del->setFont(FONT);
    for (Int i = 0; i < NUM_CHARACTER; i++)
    {
        id[i]->setFont(FONT);
        motion[i]->setFont(FONT);
        face[i]->setFont(FONT);
        id_label[i]->setFont(FONT);
        motion_label[i]->setFont(FONT);
        face_label[i]->setFont(FONT);
    }
    title_label->setFont(FONT);
    text_label->setFont(FONT);
    speaker_label->setFont(FONT);
    music_label->setFont(FONT);
    background_label->setFont(FONT);
    title->setFont(FONT);
    text->setFont(FONT);
    speaker->setFont(FONT);
    music->setFont(FONT);
    background->setFont(FONT);
    status->setFont(FONT);

    path_label->setText("Filename:");
    path->setText(filename);
    load->setText("Load");
    save->setText("Save");
    list_label->setText("Scene");
    move_up->setText("Move Up");
    move_down->setText("Move Down");
    add->setText("New Scene");
    del->setText("Delete");
    for (Int i = 0; i < NUM_CHARACTER; i++)
    {
        id_label[i]->setText(QString("Character %1: ID").arg(i));
        motion_label[i]->setText(QString("Character %1: Motion").arg(i));
        face_label[i]->setText(QString("Character %1: Face").arg(i));
    }
    title_label->setText("Title");
    text_label->setText("Text");
    speaker_label->setText("Speaker: ID");
    music_label->setText("Music");
    background_label->setText("Background");

    path_label->show();
    path->show();
    load->show();
    save->show();
    list_label->show();
    list->show();
    move_up->show();
    move_down->show();
    add->show();
    del->show();
    for (Int i = 0; i < NUM_CHARACTER; i++)
    {
        id[i]->show();
        motion[i]->show();
        face[i]->show();
        id_label[i]->show();
        motion_label[i]->show();
        face_label[i]->show();
    }
    title_label->show();
    text_label->show();
    speaker_label->show();
    music_label->show();
    background_label->show();
    title->show();
    text->show();
    speaker->show();
    music->show();
    background->show();
    status->show();

    loadCompleter();

    filename = "";
    displayUI();
    connect(move_up, SIGNAL(clicked()), this, SLOT(moveUp()));
    connect(move_down, SIGNAL(clicked()), this, SLOT(moveDown()));
    connect(add, SIGNAL(clicked()), this, SLOT(addScene()));
    connect(del, SIGNAL(clicked()), this, SLOT(delScene()));
    connect(load, SIGNAL(clicked()), this, SLOT(loadJson()));
    connect(save, SIGNAL(clicked()), this, SLOT(saveJson()));
    connect(list, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(setIndex()));

    shortcut.push_back(new QShortcut(QKeySequence("Ctrl+S"), this));
    connect(shortcut.back(), SIGNAL(activated()), this, SLOT(saveJson()));
    shortcut.push_back(new QShortcut(QKeySequence("Ctrl+O"), this));
    connect(shortcut.back(), SIGNAL(activated()), this, SLOT(loadJson()));
}

MainWin::~MainWin()
{
    delete path_label;
    delete path;
    delete load;
    delete save;
    delete list_label;
    delete list;
    delete move_up;
    delete move_down;
    delete add;
    delete del;
    for (Int i = 0; i < NUM_CHARACTER; i++)
    {
        delete id[i];
        delete motion[i];
        delete face[i];
        delete id_label[i];
        delete motion_label[i];
        delete face_label[i];
    }
    delete title;
    delete text;
    delete speaker;
    delete music;
    delete background;
    delete title_label;
    delete text_label;
    delete speaker_label;
    delete music_label;
    delete background_label;
    status = new QLineEdit(this);
    for (Int i = 0; i < shortcut.size(); i++)
    {
        delete shortcut[i];
    }
}

void MainWin::loadJson()
{
    filename = QFileDialog::getOpenFileName(this, "Open File", "./");
    path->setText(filename);
    status->setText("Load Json.");
    QFile fp(filename);
    if (!fp.open(QIODevice::ReadOnly))
    {
        QMessageBox msgbox(this);
        msgbox.setWindowTitle("Error");
        msgbox.setText(QString("Can't open %1.").arg(filename));
        msgbox.exec();
        filename = "";
        path->setText("");
        return;
    }
    QByteArray data = fp.readAll();
    fp.close();
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(data, &json_error));
    if (json_error.error != QJsonParseError::NoError)
    {
        QMessageBox msgbox(this);
        msgbox.setWindowTitle("Error");
        msgbox.setText(QString("Invaild format."));
        msgbox.exec();
        filename = "";
        path->setText("");
        return;
    }
    QJsonArray json = jsonDoc.array();
    scene.clear();
    for (Int i = 0; i < json.size(); i++)
    {
        scene.push_back(Scene(json[i].toObject()));
    }
    setIndex(0);
    displayScene();
    return;
}

void MainWin::saveJson()
{
    saveScene();
    if (filename == "")
    {
        filename = QFileDialog::getSaveFileName(this, "Save File", "./");
    }
    path->setText(filename);
    QJsonArray json;
    for (Int i = 0; i < scene.size(); i++)
    {
        json.append(scene[i].save());
    }
    QFile fp(filename);
    if (!fp.open(QIODevice::ReadWrite))
    {
        QMessageBox msgbox(this);
        msgbox.setWindowTitle("Error");
        msgbox.setText(QString("Can't open %1.").arg(filename));
        msgbox.exec();
        filename = "";
        path->setText("");
        return;
    }
    QJsonDocument jsonDoc;
    jsonDoc.setArray(json);
    fp.write(jsonDoc.toJson());
    fp.close();
    status->setText("Save Json.");
    return;
}

void MainWin::saveScene()
{
    for (Int i = 0; i < NUM_CHARACTER; i++)
    {
        scene[index].character_id[i] = id[i]->text();
        scene[index].character_motion[i] = motion[i]->text();
        scene[index].character_face[i] = face[i]->text();
    }
    scene[index].title = title->text();
    scene[index].text = text->toPlainText();
    scene[index].speaker = speaker->text();
    scene[index].music = music->text();
    scene[index].background = background->text();
    status->setText("Save Scene.");
    return;
}

void MainWin::setIndex()
{
    saveScene();
    setIndex(list->currentRow());
    displayScene();
    return;
}

void MainWin::setIndex(const Int &n)
{
    index = qMax(0, n);
    index = qMin(scene.size() - 1, n);
    list_label->setText(QString("Scene: %1/%2").arg(index + 1).arg(scene.size()));
    list->setCurrentRow(index);
    return;
}

void MainWin::displayUI()
{
    const Int WIDTH = width();
    const Int HEIGHT = height();

    const Int WIDTH_GAP = WIDTH / 32;
    const Int HEIGHT_GAP = HEIGHT / 32;

    const Int WIDTH_UNIT = (WIDTH - 7 * WIDTH_GAP) / 6;
    const Int HEIGHT_UNIT = HEIGHT / 16;

    path_label->resize(WIDTH_UNIT / 2, HEIGHT_UNIT);
    path_label->move(WIDTH_GAP, HEIGHT_GAP);
    path->resize(WIDTH - 4 * WIDTH_GAP - 2.5 * WIDTH_UNIT, HEIGHT_UNIT);
    path->move(WIDTH_UNIT / 2 + WIDTH_GAP, HEIGHT_GAP);
    path->setReadOnly(true);

    load->resize(WIDTH_UNIT, HEIGHT_UNIT);
    load->move(WIDTH - 2 * WIDTH_GAP - 2 * WIDTH_UNIT, HEIGHT_GAP);

    save->resize(WIDTH_UNIT, HEIGHT_UNIT);
    save->move(WIDTH - WIDTH_GAP - WIDTH_UNIT, HEIGHT_GAP);

    list_label->resize(2 * WIDTH_UNIT + WIDTH_GAP, HEIGHT_UNIT / 2);
    list_label->move(WIDTH_GAP, 2 * HEIGHT_GAP + HEIGHT_UNIT);
    list->resize(2 * WIDTH_UNIT + WIDTH_GAP, HEIGHT - 3.5 * HEIGHT_UNIT - 5 * HEIGHT_GAP);
    list->move(WIDTH_GAP, 2 * HEIGHT_GAP + 1.5 * HEIGHT_UNIT);

    move_up->resize(WIDTH_UNIT, HEIGHT_UNIT);
    move_up->move(WIDTH_GAP, HEIGHT - 2 * HEIGHT_GAP - 2 * HEIGHT_UNIT);

    move_down->resize(WIDTH_UNIT, HEIGHT_UNIT);
    move_down->move(2 * WIDTH_GAP + WIDTH_UNIT, HEIGHT - 2 * HEIGHT_GAP - 2 * HEIGHT_UNIT);

    add->resize(WIDTH_UNIT, HEIGHT_UNIT);
    add->move(WIDTH_GAP, HEIGHT - HEIGHT_GAP - HEIGHT_UNIT);

    del->resize(WIDTH_UNIT, HEIGHT_UNIT);
    del->move(2 * WIDTH_GAP + WIDTH_UNIT, HEIGHT - HEIGHT_GAP - HEIGHT_UNIT);

    for (Int i = 0; i < NUM_CHARACTER; i++)
    {
        id_label[i]->resize(WIDTH_UNIT, HEIGHT_UNIT / 2);
        id_label[i]->move(WIDTH - (4 - i) * (WIDTH_UNIT + WIDTH_GAP), HEIGHT_UNIT + 2 * HEIGHT_GAP);
        id[i]->resize(WIDTH_UNIT, HEIGHT_UNIT);
        id[i]->move(WIDTH - (4 - i) * (WIDTH_UNIT + WIDTH_GAP), 1.5 * HEIGHT_UNIT + 2 * HEIGHT_GAP);

        motion_label[i]->resize(WIDTH_UNIT, HEIGHT_UNIT / 2);
        motion_label[i]->move(WIDTH - (4 - i) * (WIDTH_UNIT + WIDTH_GAP), 2.5 * HEIGHT_UNIT + 2 * HEIGHT_GAP);
        motion[i]->resize(WIDTH_UNIT, HEIGHT_UNIT);
        motion[i]->move(WIDTH - (4 - i) * (WIDTH_UNIT + WIDTH_GAP), 3 * HEIGHT_UNIT + 2 * HEIGHT_GAP);

        face_label[i]->resize(WIDTH_UNIT, HEIGHT_UNIT / 2);
        face_label[i]->move(WIDTH - (4 - i) * (WIDTH_UNIT + WIDTH_GAP), 4 * HEIGHT_UNIT + 2 * HEIGHT_GAP);
        face[i]->resize(WIDTH_UNIT, HEIGHT_UNIT);
        face[i]->move(WIDTH - (4 - i) * (WIDTH_UNIT + WIDTH_GAP), 4.5 * HEIGHT_UNIT + 2 * HEIGHT_GAP);
    }

    title_label->resize(4 * WIDTH_UNIT + 3 * WIDTH_GAP, HEIGHT_UNIT / 2);
    title_label->move(WIDTH - 4 * (WIDTH_GAP + WIDTH_UNIT), 3 * HEIGHT_GAP + 5.5 * HEIGHT_UNIT);
    title->resize(4 * WIDTH_UNIT + 3 * WIDTH_GAP, HEIGHT_UNIT);
    title->move(WIDTH - 4 * (WIDTH_GAP + WIDTH_UNIT), 3 * HEIGHT_GAP + 6 * HEIGHT_UNIT);

    text_label->resize(4 * WIDTH_UNIT + 3 * WIDTH_GAP, HEIGHT_UNIT / 2);
    text_label->move(WIDTH - 4 * (WIDTH_GAP + WIDTH_UNIT), 4 * HEIGHT_GAP + 7 * HEIGHT_UNIT);
    text->resize(4 * WIDTH_UNIT + 3 * WIDTH_GAP, HEIGHT - 8.5 * HEIGHT_UNIT - 6 * HEIGHT_GAP);
    text->move(WIDTH - 4 * (WIDTH_GAP + WIDTH_UNIT), 4 * HEIGHT_GAP + 7.5 * HEIGHT_UNIT);

    speaker_label->resize(WIDTH_UNIT, HEIGHT_UNIT / 2);
    speaker_label->move(WIDTH - WIDTH_GAP - WIDTH_UNIT, HEIGHT_UNIT + 2 * HEIGHT_GAP);
    speaker->resize(WIDTH_UNIT, HEIGHT_UNIT);
    speaker->move(WIDTH - WIDTH_GAP - WIDTH_UNIT, 1.5 * HEIGHT_UNIT + 2 * HEIGHT_GAP);

    music_label->resize(WIDTH_UNIT, HEIGHT_UNIT / 2);
    music_label->move(WIDTH - WIDTH_GAP - WIDTH_UNIT, 2.5 * HEIGHT_UNIT + 2 * HEIGHT_GAP);
    music->resize(WIDTH_UNIT, HEIGHT_UNIT);
    music->move(WIDTH - WIDTH_GAP - WIDTH_UNIT, 3 * HEIGHT_UNIT + 2 * HEIGHT_GAP);

    background_label->resize(WIDTH_UNIT, HEIGHT_UNIT / 2);
    background_label->move(WIDTH - WIDTH_GAP - WIDTH_UNIT, 4 * HEIGHT_UNIT + 2 * HEIGHT_GAP);
    background->resize(WIDTH_UNIT, HEIGHT_UNIT);
    background->move(WIDTH - WIDTH_GAP - WIDTH_UNIT, 4.5 * HEIGHT_UNIT + 2 * HEIGHT_GAP);

    status->resize(4 * WIDTH_UNIT + 3 * WIDTH_GAP, HEIGHT_UNIT);
    status->move(WIDTH - 4 * (WIDTH_GAP + WIDTH_UNIT), HEIGHT - HEIGHT_GAP - HEIGHT_UNIT);

    return;
}

void MainWin::displayScene()
{
    list->clear();
    for (int i = 0; i < scene.size(); i++)
    {
        list->addItem(QString("Scene %1: %2").arg(i + 1).arg(scene[i].title));
    }
    if (0 <= index && index < scene.size())
    {
        list->setCurrentRow(index);
    }
    for (Int i = 0; i < NUM_CHARACTER; i++)
    {
        id[i]->setText("");
        motion[i]->setText("");
        face[i]->setText("");
    }
    title->setText("");
    text->setText("");
    speaker->setText("");
    music->setText("");
    background->setText("");
    if (0 <= index && index < scene.size())
    {
        for (Int i = 0; i < NUM_CHARACTER; i++)
        {
            id[i]->setText(scene.at(index).character_id[i]);
            motion[i]->setText(scene.at(index).character_motion[i]);
            face[i]->setText(scene.at(index).character_face[i]);
        }
        title->setText(scene.at(index).title);
        text->setText(scene.at(index).text);
        speaker->setText(scene.at(index).speaker);
        music->setText(scene.at(index).music);
        background->setText(scene.at(index).background);
    }
    return;
}

void MainWin::moveUp()
{
    if (0 < index && index < scene.size())
    {
        saveScene();
        qSwap(scene[index], scene[index - 1]);
        setIndex(index - 1);
        status->setText("Move Up.");
    }
    displayScene();
    return;
}

void MainWin::moveDown()
{
    if (0 <= index && index < (scene.size() - 1))
    {
        saveScene();
        qSwap(scene[index], scene[index + 1]);
        setIndex(index + 1);
        status->setText("Move Down.");
    }
    displayScene();
    return;
}

void MainWin::addScene()
{
    if (0 <= index && index < scene.size())
    {
        saveScene();
        auto it = scene.begin() + index + 1;
        scene.insert(it, Scene());
        setIndex(index + 1);
        status->setText("Insert New Scene.");
    }
    else
    {
        scene.push_back(Scene());
        setIndex(scene.size());
        status->setText("Add New Scene.");
    }
    displayScene();
    return;
}

void MainWin::delScene()
{
    if (0 <= index && index < scene.size())
    {
        auto it = scene.begin() + index;
        scene.erase(it);
        setIndex(index);
        status->setText("Delete Scene.");
    }
    displayScene();
    return;
}

void MainWin::loadCompleter()
{
    QStringList list;
    QFile fid("id.txt");
    if (fid.open(QIODevice::ReadWrite))
    {
        while (!fid.atEnd())
        {
            list.push_back(fid.readLine());
        }
        fid.close();
    }
    id_completer = new QCompleter(list, this);
    id_completer->setCaseSensitivity(Qt::CaseInsensitive);

    list.clear();
    QFile fmotion("motion.txt");
    if (fmotion.open(QIODevice::ReadWrite))
    {
        while (!fmotion.atEnd())
        {
            list.push_back(fmotion.readLine());
        }
        fmotion.close();
    }
    motion_completer = new QCompleter(list, this);
    motion_completer->setCaseSensitivity(Qt::CaseInsensitive);

    list.clear();
    QFile fface("face.txt");
    if (fface.open(QIODevice::ReadWrite))
    {
        while (!fface.atEnd())
        {
            list.push_back(fface.readLine());
        }
        fface.close();
    }
    face_completer = new QCompleter(list, this);
    face_completer->setCaseSensitivity(Qt::CaseInsensitive);

    list.clear();
    QFile fmusic("music.txt");
    if (fmusic.open(QIODevice::ReadWrite))
    {
        while (!fmusic.atEnd())
        {
            list.push_back(fmusic.readLine());
        }
        fmusic.close();
    }
    music_completer = new QCompleter(list, this);
    music_completer->setCaseSensitivity(Qt::CaseInsensitive);

    list.clear();
    QFile fbackground("background.txt");
    if (fbackground.open(QIODevice::ReadWrite))
    {
        while (!fbackground.atEnd())
        {
            list.push_back(fbackground.readLine());
        }
        fbackground.close();
    }
    background_completer = new QCompleter(list, this);
    background_completer->setCaseSensitivity(Qt::CaseInsensitive);
    for (Int i = 0; i < NUM_CHARACTER; i++)
    {
        id[i]->setCompleter(id_completer);
        motion[i]->setCompleter(motion_completer);
        face[i]->setCompleter(face_completer);
    }
    speaker->setCompleter(id_completer);
    music->setCompleter(music_completer);
    background->setCompleter(background_completer);
    return;
}

void MainWin::resizeEvent(QResizeEvent *event)
{
    if (width() < MIN_WIDTH)
    {
        resize(MIN_WIDTH, height());
    }
    if (height() < MIN_HEIGHT)
    {
        resize(width(), MIN_HEIGHT);
    }
    status->setText("Resize.");
    displayUI();
    return;
}
