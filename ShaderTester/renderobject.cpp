#include "renderobject.h"

RenderObject::RenderObject(QString name, Mesh* mesh) :
    m_name(name),
    m_mesh(mesh),
    m_shader(NULL)
{

}

RenderObject::~RenderObject()
{
    delete m_mesh;
}

QString RenderObject::getName() {
    return m_name;
}

void RenderObject::setShader(Shader* shader) {
    m_shader = shader;
}

void RenderObject::initializeGL()
{

}

void RenderObject::paintGL()
{
    if(m_shader != NULL) {
        m_shader->beginGL();
    }

    m_mesh->draw();
}

