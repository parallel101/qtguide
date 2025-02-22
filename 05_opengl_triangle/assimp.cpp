#include "assimp.h"
#include <QDebug>
#include <fstream>
#include <QtMath>
#include <sstream>
#include <QVector3D>
#include <array>


static QVector3D computeNormalBiased(QVector3D const &a, QVector3D const &b,
                                       QVector3D const &c) {
    // 计算三角形法线，带asin项加权的版本
    QVector3D ab = b - a;
    QVector3D ac = c - a;
    QVector3D n = QVector3D::crossProduct(ab, ac);
    auto nlen = n.length();
    if (nlen != 0) [[likely]] {
        auto labc = ab.length() * ac.length();
        if (labc >= nlen) [[likely]] {
            n *= qAsin(nlen / labc) / nlen;
        } else {
            n *= 1.0f / nlen;
        }
    }
    return n;
}

auto Assimp::loadObj(QString const &path) -> std::vector<Vertex>
{
    std::ifstream file(path.toStdString());
    if (!file.is_open()) {
        qWarning() << "Failed to open file:" << path;
        return {};
    }

    std::vector<Vertex> vertices; // VBO
    std::vector<std::array<quint32, 3>> faces; // EBO
    // glDrawElements

    // 编辑 / 建模软件：多边形模型 + EBO 式的带拓扑连接的
    // 渲染 / 游戏引擎：三角形模型 + VBO 直接渲染（无拓扑）

    std::string line;
    while (std::getline(file, line)) {
        if (line.substr(0, 2) == "v ") {
            std::istringstream s(line.substr(2));
            QVector3D vertex;
            s >> vertex[0] >> vertex[1] >> vertex[2];
            vertices.push_back({vertex, {}});

        } else if (line.substr(0, 2) == "f ") {
            std::istringstream s(line.substr(2));
            std::string splitted;
            std::vector<quint32> indices;
            while (std::getline(s, splitted, ' ')) {
                quint32 index = 1;
                std::istringstream(splitted) >> index;
                indices.push_back(index - 1);
            }
            for (size_t i = 2; i < indices.size(); i++) {
                std::array<quint32, 3> face{indices[0], indices[i - 1], indices[i]};
                faces.push_back(face);
            }
        }
    }

    file.close();
    qDebug() << path << "loaded" << vertices.size() << "vertices" << faces.size() << "faces";

    for (auto const &face: faces) {
        auto &a = vertices[face[0]];
        auto &b = vertices[face[1]];
        auto &c = vertices[face[2]];
        a.normal += computeNormalBiased(a.position, b.position, c.position);
        b.normal += computeNormalBiased(b.position, c.position, a.position);
        c.normal += computeNormalBiased(c.position, a.position, b.position);
    }
    for (auto &v: vertices) {
        v.normal.normalize();
    }

    std::vector<Vertex> flatVertices;
    flatVertices.reserve(faces.size() * 3);
    for (auto const &face: faces) {
        flatVertices.push_back(vertices[face[0]]);
        flatVertices.push_back(vertices[face[1]]);
        flatVertices.push_back(vertices[face[2]]);
    }
    return flatVertices;
}
