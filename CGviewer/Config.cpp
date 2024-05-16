#include "Config.h"

QVector<QString> listDir(const QString &path)
{
    QVector<QString> res;
    QFileInfo info(path);
    QDir dir(path);
    QStringList tmp = dir.entryList(QDir::Files);
    for (Int i = 0; i < tmp.size(); i++)
    {
        QFileInfo info(tmp[i]);
        if (info.isFile())
        {
            res.push_back(tmp[i]);
        }
    }
    res.sort(Qt::CaseInsensitive);
    return res;
}

QImage loadFigure()
{
    QImage res;

    return res;
}

Figure::Figure(const QString &path)
{
    this->path = path;
}