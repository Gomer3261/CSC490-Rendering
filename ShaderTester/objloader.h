#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <QList>
#include "mesh.h"
#include "material.h"
#include "renderobject.h"

class ObjLoader
{
public:
    ObjLoader(QString path, QString root);
    ~ObjLoader();

    RenderObject* popObject();
    bool hasObject();
private:
    QMap<QString, Material::Ptr> m_materials;
    QList<RenderObject*> m_objects;
    QString m_object_root;

    int m_vertexOffset;
    int m_uvOffset;
    int m_normalOffset;
    int m_vertexCount;
    int m_uvCount;
    int m_normalCount;

    void parseObjFile(QString path);
};

#endif // OBJLOADER_H
