#include "objmaterialloader.h"

void ObjMaterialLoader::parseObjMtlFile(QString path, QMap<QString, Material::Ptr> *material_dict)
{
    QByteArray byteArray = path.toUtf8();
    FILE * file = fopen(byteArray.constData(), "r");
    if( file == NULL ){
        printf("Impossible to open the file !\n");
        return;
    }

    Material::Ptr material = Material::create();

    for(;;) {
        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break;

        if( strcmp( lineHeader, "newmtl" ) == 0 ) {
            char name[128];
            fscanf(file, "%s\n", name);
            QString s_name = QString(name);

            material = Material::create();
            material_dict->insert(s_name, material);
        }
        else if ( strcmp( lineHeader, "Ns" ) == 0 )
        {
            float shininess;
            fscanf(file, "%f\n", &shininess);
            material->setShininess(shininess/1000.0f);
        }
        else if ( strcmp( lineHeader, "Ka" ) == 0 )
        {
            float r; float g; float b;
            fscanf(file, "%f %f %f\n", &r, &g, &b );
            material->setAmbient(r, g, b);
        }
        else if ( strcmp( lineHeader, "Kd" ) == 0 )
        {
            float r; float g; float b;
            fscanf(file, "%f %f %f\n", &r, &g, &b );
            material->setDiffuse(r, g, b);
        }
        else if ( strcmp( lineHeader, "map_Kd" ) == 0 )
        {
            char image_file[128];
            fscanf(file, "%s\n", image_file);

            QString string_file = path.left(path.lastIndexOf('/')+1);
            string_file += image_file;

            QImage image = QImage(string_file);
            material->setDiffuseTexture(image);
        }
        else if ( strcmp( lineHeader, "Ks" ) == 0 )
        {
            float r; float g; float b;
            fscanf(file, "%f %f %f\n", &r, &g, &b );
            material->setSpecular(r, g, b);
        }
        else if ( strcmp( lineHeader, "map_Ks") == 0)
        {
            char image_file[128];
            fscanf(file, "%s\n", image_file);
            QString string_file = path.left(path.lastIndexOf('/')+1);
            string_file += image_file;

            QImage image = QImage(string_file);
            material->setSpecularTexture(image);
        }
        else if ( strcmp( lineHeader, "Ke" ) == 0 )
        {
            float r; float g; float b;
            fscanf(file, "%f %f %f\n", &r, &g, &b );
            material->setEmission(r, g, b);
        }
        else if ( strcmp( lineHeader, "d" ) == 0 )
        {
            float dissolve;
            fscanf(file, "%f\n", &dissolve);
            material->setDissolve(dissolve);
        }
        else if ( strcmp( lineHeader, "Ni" ) == 0 )
        {
            float density;
            fscanf(file, "%f\n", &density);
            material->setOpticalDensity(density);
        }
        else if ( strcmp( lineHeader, "alpha" ) == 0 )
        {
            float alpha;
            fscanf(file, "%f\n", &alpha);
            material->setAlpha(alpha);
        }
        else if ( strcmp( lineHeader, "illum" ) == 0 )
        {
            int model;
            fscanf(file, "%d\n", &model);
            material->setIlluminationModel(model);
        }
        else if ( strcmp( lineHeader, "map_Bump") == 0)
        {
            char image_file[128];
            fscanf(file, "%s\n", image_file);
            QString string_file = path.left(path.lastIndexOf('/')+1);
            string_file += image_file;

            QImage image = QImage(string_file);
            material->setBumpTexture(image);
        }
        else if ( strcmp( lineHeader, "map_Normal") == 0)
        {
            char image_file[128];
            fscanf(file, "%s\n", image_file);
            QString string_file = path.left(path.lastIndexOf('/')+1);
            string_file += image_file;

            QImage image = QImage(string_file);
            material->setNormalTexture(image);
        }
    }
}

