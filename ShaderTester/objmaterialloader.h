#ifndef OBJMATERIALLOADER_H
#define OBJMATERIALLOADER_H

#include <QList>
#include "material.h"

class ObjMaterialLoader
{
public:
    static void parseObjMtlFile(QString path, QMap<QString, Material::Ptr> *material_dict);
};

#endif // OBJMATERIALLOADER_H
