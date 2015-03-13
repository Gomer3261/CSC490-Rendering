#ifndef OBJECT_H
#define OBJECT_H

#include "mesh.h"
#include "shader.h"

class RenderObject
{
public:
    RenderObject(QString name, Mesh* mesh);
    ~RenderObject();

    QString getName();

    void initializeGL();
    void paintGL();
private:
    QString m_name;
    Mesh* m_mesh;
};

#endif // OBJECT_H
