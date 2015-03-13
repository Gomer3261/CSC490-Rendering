#include "renderobject.h"

RenderObject::RenderObject(QString name, Mesh* mesh) :
    m_name(name),
    m_mesh(mesh)
{

}

RenderObject::~RenderObject()
{
    delete m_mesh;
}

QString RenderObject::getName() {
    return m_name;
}

void RenderObject::initializeGL()
{

}

void RenderObject::paintGL()
{
    m_mesh->draw();
}

