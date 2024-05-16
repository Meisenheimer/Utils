#include "Scene.h"

Scene::Scene()
{
    for (Int i = 0; i < NUM_CHARACTER; i++)
    {
        character_id[i] = "";
        character_motion[i] = "";
        character_face[i] = "";
    }
    title = "";
    text = "";
    speaker = "";
    music = "";
    background = "";
}

Scene::Scene(const QJsonObject &json)
{
    load(json);
}

void Scene::load(const QJsonObject &json)
{
    QJsonArray id = json.value("id").toArray();
    QJsonArray motion = json.value("motion").toArray();
    QJsonArray face = json.value("face").toArray();

    for (Int i = 0; i < NUM_CHARACTER; i++)
    {
        character_id[i] = id[i].toString();
        character_motion[i] = motion[i].toString();
        character_face[i] = face[i].toString();
    }

    title = json.value("title").toString();
    text = json.value("text").toString();
    speaker = json.value("speaker").toString();
    music = json.value("music").toString();
    background = json.value("background").toString();
    return;
}

QJsonObject Scene::save() const
{
    QJsonObject res;

    QJsonArray id;
    QJsonArray motion;
    QJsonArray face;

    for (Int i = 0; i < NUM_CHARACTER; i++)
    {
        id.append(character_id[i]);
        motion.append(character_motion[i]);
        face.append(character_face[i]);
    }
    res.insert("id", id);
    res.insert("motion", motion);
    res.insert("face", face);
    res.insert("title", title);
    res.insert("text", text);
    res.insert("speaker", speaker);
    res.insert("music", music);
    res.insert("background", background);

    return res;
}