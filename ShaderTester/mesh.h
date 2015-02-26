#ifndef MESH_H
#define MESH_H

#include "qgl.h"
#include <QList>
#include <QVector2D>
#include <QVector3D>
#include <QString>
#include <QDebug>

#include "material.h"

class Mesh
{
public:
    Mesh(Material::Ptr material);
    ~Mesh();

    void addVertex(float x, float y, float z);
    void addUV(float u, float v);
    void addNormal(float x, float y, float z);
    void addTriangle(unsigned int vertexIndex[3], unsigned int uvIndex[3], unsigned int normalIndex[3]);
    void finalize();

    void draw();
private:
    Material::Ptr m_material;
    GLuint m_vertexBuffer;

    QList<QVector3D*> m_vertexData;
    QList<QVector2D*> m_uvData;
    QList<QVector3D*> m_normalData;
    QList<unsigned int> m_vertexIndicies;
    QList<unsigned int> m_uvIndicies;
    QList<unsigned int> m_normalIndicies;

    int m_vertexCount;

    void generateVBO(float* vertices, float* uvs, float* normals);
    void clearLoader();
    void clearVBO();
};

#endif // MESH_H
