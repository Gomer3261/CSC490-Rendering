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

    void setShader(Shader* shader);
    void initializeGL();
    void paintGL();
private:
    QString m_name;
    Mesh* m_mesh;
    Shader* m_shader;
    //TODO Materials
};

#endif // OBJECT_H
