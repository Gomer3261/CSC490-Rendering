#ifndef MESH_H
#define MESH_H

#include "qgl.h"
#include <QList>
#include <QVector2D>
#include <QVector3D>
#include <QString>
#include <QDebug>
#include <QMultiHash>

#include "material.h"
#include "shadermanager.h"

#define NBITS       5
#define BMASK       0x037
#define HASH(a, b, c)  (( (a&BMASK)<<NBITS|(b&BMASK) )<<NBITS|(c&BMASK))
#define HSIZE       1<<NBITS*3

class Mesh
{
public:
    Mesh(Material::Ptr material);
    ~Mesh();

    class MeshVertex;

    void addVertex(float x, float y, float z);
    void addUV(float u, float v);
    void addNormal(float x, float y, float z);
    void addTriangle(int vertexIndex[3], int uvIndex[3], int normalIndex[3]);
    void setMaterial(Material::Ptr material);
    void finalize();

    void draw();
private:
    Material::Ptr m_material;
    GLuint m_vertexBuffer;
    GLuint m_indexBuffer;

    QList<QVector3D*> m_vertexData;
    QList<QVector2D*> m_uvData;
    QList<QVector3D*> m_normalData;
    QList<unsigned int> m_vertexIndicies;
    QList<unsigned int> m_uvIndicies;
    QList<unsigned int> m_normalIndicies;

    QMultiHash<uint, Mesh::MeshVertex*> m_vertexTable;

    int m_vertexCount;
    int m_vertexDataCount;

    void generateVBO(float* vertices, float* uvs, float* normals, int* vertex_indices);
    void clearLoader();
    void clearVBO();

public:
    class MeshVertex
    {
    public:
        MeshVertex(int vert, int uv, int norm) :
            m_vertex_index(vert),
            m_uv_index(uv),
            m_normal_index(norm),
            m_final_index(-1)
        {}

        int vertex_index() const { return m_vertex_index; }
        int uv_index() const { return m_uv_index; }
        int normal_index() const { return m_normal_index; }
        int final_index() { return m_final_index; }
        void setFinalIndex(int index) { m_final_index = index; }

    private:
        int m_vertex_index;
        int m_uv_index;
        int m_normal_index;
        int m_final_index;
    };
};

inline bool operator==(const Mesh::MeshVertex &v1, const Mesh::MeshVertex &v2)
{
    return v1.vertex_index() == v2.vertex_index()
           && v1.uv_index() == v2.uv_index()
           && v1.normal_index() == v2.normal_index();
}

inline uint qHash(const Mesh::MeshVertex &key)
{
    return HASH(key.vertex_index(), key.uv_index(), key.normal_index());
}

#endif // MESH_H
