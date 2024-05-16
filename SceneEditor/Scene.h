#ifndef SCENE_H
#define SCENE_H

#include "Config.h"

constexpr Int LEFT = 0;
constexpr Int MIDDLE = 0;
constexpr Int RIGHT = 0;

struct Scene
{
    QString character_id[NUM_CHARACTER];
    QString character_motion[NUM_CHARACTER];
    QString character_face[NUM_CHARACTER];

    QString title;
    QString text;
    QString speaker;
    QString music;
    QString background;

    Scene();
    Scene(const QJsonObject &json);

    void load(const QJsonObject &json);
    QJsonObject save() const;
};

#endif // SCENE_H
