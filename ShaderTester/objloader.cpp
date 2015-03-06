#include "objloader.h"

ObjLoader::ObjLoader(QString path, QString root) :
    m_materials(),
    m_objects(),
    m_object_root(root),
    m_vertexOffset(1),
    m_uvOffset(1),
    m_normalOffset(1),
    m_vertexCount(0),
    m_uvCount(0),
    m_normalCount(0)
{
    m_materials.insert(QString("default"), Material::create());
    parseObjFile(path);
}

ObjLoader::~ObjLoader()
{

}

RenderObject* ObjLoader::popObject() {
    return m_objects.takeFirst();
}

bool ObjLoader::hasObject() {
    return !m_objects.empty();
}

void ObjLoader::parseObjFile(QString path)
{
    QByteArray byteArray = path.toUtf8();
    FILE * file = fopen(byteArray.constData(), "r");
    if( file == NULL ){
        printf("Impossible to open the file !\n");
        return;
    }

    Mesh* mesh = NULL;
    RenderObject* object = NULL;

    for(;;) {
        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break;

        if( strcmp( lineHeader, "o" ) == 0 ) {
            if(mesh != NULL && object != NULL) {
                mesh->finalize();
                m_objects.push_back(object);
            }

            char name[128];
            fscanf(file, "%s\n", name);
            QString s_name = QString();
            s_name.append(m_object_root);
            s_name.append(name);

            mesh = new Mesh(Material::Ptr(m_materials.find(QString("default")).value()));
            object= new RenderObject(s_name, mesh);

            m_vertexOffset += m_vertexCount;
            m_uvOffset += m_uvCount;
            m_normalOffset += m_normalCount;
            m_vertexCount = 0;
            m_uvCount = 0;
            m_normalCount = 0;
        }
        else if(strcmp( lineHeader, "mtllib" ) == 0)
        {
            char file_name[128];
            fscanf(file, "%s\n", file_name);

            QString mtl_file = path.left(path.lastIndexOf('/')+1);
            mtl_file += file_name;

            ObjMaterialLoader::parseObjMtlFile(mtl_file, &m_materials);
        }
        else if(mesh != NULL) {
            if ( strcmp( lineHeader, "usemtl" ) == 0 )
            {
                char mtl_name[64];
                fscanf(file, "%s\n", mtl_name);

                QString mtl_string = QString(mtl_name);

                if(m_materials.contains(mtl_string)) {
                    mesh->setMaterial(m_materials.find(mtl_string).value());
                }
            }
            else if ( strcmp( lineHeader, "v" ) == 0 )
            {
                float x; float y; float z;
                fscanf(file, "%f %f %f\n", &x, &y, &z );
                mesh->addVertex(x, y, z);
                m_vertexCount++;
            }
            else if ( strcmp( lineHeader, "vt" ) == 0 )
            {
                float u; float v;
                fscanf(file, "%f %f\n", &u, &v );
                mesh->addUV(u, v);
                m_uvCount++;
            }
            else if ( strcmp( lineHeader, "vn" ) == 0 )
            {
                float x; float y; float z;
                fscanf(file, "%f %f %f\n", &x, &y, &z );
                mesh->addNormal(x, y, z);
                m_normalCount++;
            }
            else if ( strcmp( lineHeader, "f" ) == 0 )
            {
                unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
                int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
                if (matches != 9){
                    printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                    return;
                }

                /* Indexes in an OBJ file start at 1, so we must reduce all indexes by 1 */
                for (int i=0; i<3; i++) {
                    vertexIndex[i] = vertexIndex[i]-m_vertexOffset;
                    uvIndex[i] = uvIndex[i]-m_uvOffset;
                    normalIndex[i] = normalIndex[i]-m_normalOffset;
                }
                mesh->addTriangle(vertexIndex, uvIndex, normalIndex);
            }
        }
    }

    if(mesh != NULL && object != NULL) {
        mesh->finalize();
        m_objects.push_back(object);
    }
}
