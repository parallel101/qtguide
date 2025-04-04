#pragma once

#include <QString>
#include <QVector3D>
#include <vector>

struct Assimp
{
    struct Vertex {
        QVector3D position;
        QVector3D normal;
    };

    // position | normal | position

    static std::vector<Vertex> loadObj(QString const &path);
};
