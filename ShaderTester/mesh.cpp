#include "mesh.h"

Mesh::Mesh(Material::Ptr material) :
    m_material(material),
    m_vertexBuffer(0),
    m_indexBuffer(0),
    m_vertexData(),
    m_uvData(),
    m_normalData(),
    m_vertexIndicies(),
    m_uvIndicies(),
    m_normalIndicies(),
    m_vertexTable()
{
    m_vertexCount = 0;
    m_vertexDataCount = 0;
}

Mesh::~Mesh()
{
    clearLoader();
    clearVBO();
}

void Mesh::clearLoader() {
    /* Clean data */
    for(int i=0; i<m_vertexData.size(); i++) {
        delete m_vertexData[i];
    }
    m_vertexData.clear();
    for(int i=0; i<m_uvData.size(); i++) {
        delete m_uvData[i];
    }
    m_uvData.clear();
    for(int i=0; i<m_normalData.size(); i++) {
        delete m_normalData[i];
    }
    m_normalData.clear();
    for(QMultiHash<uint, Mesh::MeshVertex*>::iterator iter = m_vertexTable.begin(); iter != m_vertexTable.end(); ++iter) {
        delete iter.value();
    }
    m_vertexTable.clear();
}

void Mesh::clearVBO() {
    glDeleteBuffers(1, &m_vertexBuffer);
    glDeleteBuffers(1, &m_indexBuffer);
    m_vertexBuffer = 0;
    m_indexBuffer = 0;
}

void Mesh::addVertex(float x, float y, float z) {
    if(m_vertexBuffer == 0) {
        m_vertexData.push_back(new QVector3D(x, y, z));
    }
}

void Mesh::addUV(float u, float v) {
    if(m_vertexBuffer == 0) {
        m_uvData.push_back(new QVector2D(u, v));
    }
}

void Mesh::addNormal(float x, float y, float z) {
    if(m_vertexBuffer == 0) {
        m_normalData.push_back(new QVector3D(x, y, z));
    }
}

void Mesh::addTriangle(int vertexIndex[3], int uvIndex[3], int normalIndex[3]) {
    if(m_vertexBuffer == 0) {
        m_vertexCount += 3;

        m_vertexIndicies.push_back(vertexIndex[0]);
        m_vertexIndicies.push_back(vertexIndex[1]);
        m_vertexIndicies.push_back(vertexIndex[2]);
        m_uvIndicies.push_back((uvIndex[0] != -1) ? uvIndex[0] : m_uvData.length() );
        m_uvIndicies.push_back((uvIndex[1] != -1) ? uvIndex[1] : m_uvData.length() );
        m_uvIndicies.push_back((uvIndex[2] != -1) ? uvIndex[2] : m_uvData.length() );
        m_normalIndicies.push_back(normalIndex[0]);
        m_normalIndicies.push_back(normalIndex[1]);
        m_normalIndicies.push_back(normalIndex[2]);
    }
}

void Mesh::setMaterial(Material::Ptr material)
{
    m_material = material;
}

void Mesh::finalize() {
    qDebug() << "Finalizing Model:" << m_vertexCount << " vertices";

    // Forces a 0,0 uv index for missing indices.
    m_uvData.push_back(new QVector2D(0, 0));

    //Allocate for worst case scenario
    //Cut the empty ends off when transfering to GPU.
    float *vertices = new float[3*m_vertexCount];
    float *uvs = new float[2*m_vertexCount];
    float *normals = new float[3*m_vertexCount];

    int *vertex_indices = new int[m_vertexCount];

    Mesh::MeshVertex* test_vertex;
    Mesh::MeshVertex* operating_vertex;
    int vertex_array_index = 0;

    for(int i=0; i<m_vertexCount; i++) {
        operating_vertex = NULL;
        test_vertex = new Mesh::MeshVertex(m_vertexIndicies[i], m_uvIndicies[i], m_normalIndicies[i]);

        // Check if the vertex exists in the hashtable.
        QMultiHash<uint, Mesh::MeshVertex*>::iterator iter = m_vertexTable.find(qHash(*test_vertex));
        while(iter != m_vertexTable.end() && iter.key() == qHash(*test_vertex)) {
            if((*iter.value()) == *test_vertex) {
                operating_vertex = iter.value();
                delete test_vertex;
                break;
            }
            ++iter;
        } // If no actual match is found, we continue to used the old index...

        // Generating a new piece of vertex data for the GPU.
        if(operating_vertex == NULL) {
            operating_vertex = test_vertex;

            vertices[(3*vertex_array_index)] = m_vertexData[operating_vertex->vertex_index()]->x();
            vertices[(3*vertex_array_index)+1] = m_vertexData[operating_vertex->vertex_index()]->y();
            vertices[(3*vertex_array_index)+2] = m_vertexData[operating_vertex->vertex_index()]->z();

            uvs[(2*vertex_array_index)] = m_uvData[operating_vertex->uv_index()]->x();
            uvs[(2*vertex_array_index)+1] = m_uvData[operating_vertex->uv_index()]->y();

            normals[(3*vertex_array_index)] = m_normalData[operating_vertex->normal_index()]->x();
            normals[(3*vertex_array_index)+1] = m_normalData[operating_vertex->normal_index()]->y();
            normals[(3*vertex_array_index)+2] = m_normalData[operating_vertex->normal_index()]->z();

            operating_vertex->setFinalIndex(vertex_array_index++);

            //Ensure new data is correctly placed in the table.
            m_vertexTable.insert(qHash(*operating_vertex), operating_vertex);
        }

        //Store final index for references vertex.
        vertex_indices[i] = operating_vertex->final_index();
    }
    //Store final vertex data count (<= vertex count)
    m_vertexDataCount = vertex_array_index;

    //Transfer data to GPU for storage.
    generateVBO(vertices, uvs, normals, vertex_indices);

    //Delete data off CPU.
    delete vertices;
    delete uvs;
    delete normals;
    delete vertex_indices;
}

void Mesh::generateVBO(float* vertices, float* uvs, float* normals, int* vertex_indices) {
    if(m_vertexBuffer == 0) {
        qDebug() << "Creating VBO pointer";
        glGenBuffers(1, &m_vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
        glGenBuffers(1, &m_indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
    } else {
        qDebug() << "Freeing VBO pointer";
        glDeleteBuffers(1, &m_vertexBuffer);
        glGenBuffers(1, &m_vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
        glDeleteBuffers(1, &m_indexBuffer);
        glGenBuffers(1, &m_indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
    }
    glBufferData(GL_ARRAY_BUFFER, 8*m_vertexDataCount*sizeof(float), 0, GL_DYNAMIC_DRAW); //GL_STATIC_DRAW
    glBufferSubData(GL_ARRAY_BUFFER, 0, 3*m_vertexDataCount*sizeof(float), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, 3*m_vertexDataCount*sizeof(float), 2*m_vertexDataCount*sizeof(float), uvs);
    glBufferSubData(GL_ARRAY_BUFFER, 5*m_vertexDataCount*sizeof(float), 3*m_vertexDataCount*sizeof(float), normals);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vertexCount*sizeof(int), vertex_indices, GL_STATIC_DRAW);
}

/* ===================================================
 * =============== Post Initialization ===============
 * ===================================================
 */

void Mesh::draw()
{
    m_material->beginGL();
    Shader* mat_shader = ShaderManager::getInstance().getShader(m_material->getIlluminationModel());

    if(m_vertexBuffer != 0) {
        for(int i=0; i<mat_shader->passCount(); i++) {
            mat_shader->beginGL(i);

            // Bind model data buffers
            glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

            // Set data availablility flags.
            glEnableClientState(GL_NORMAL_ARRAY);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glEnableClientState(GL_VERTEX_ARRAY);

            // Describe data format.
            glNormalPointer(GL_FLOAT, 0, (void*)(5*m_vertexDataCount*sizeof(float)) );
            glTexCoordPointer(2, GL_FLOAT, 0, (void*)(3*m_vertexDataCount*sizeof(float)) );
            glVertexPointer(3, GL_FLOAT, 0, 0);

            // Draw data to buffer.
            glDrawElements(GL_TRIANGLES, m_vertexCount, GL_UNSIGNED_INT, 0);

            // Reset data availability flags.
            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            glDisableClientState(GL_NORMAL_ARRAY);

            // Unbind buffers.
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            mat_shader->endGL(i);
        }
    }

    m_material->endGL();
}
