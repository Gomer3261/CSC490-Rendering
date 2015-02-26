#include "mesh.h"

Mesh::Mesh(Material::Ptr material) :
    m_material(material),
    m_vertexBuffer(0),
    m_vertexData(),
    m_uvData(),
    m_normalData(),
    m_vertexIndicies(),
    m_uvIndicies(),
    m_normalIndicies()
{
    m_vertexCount = 0;
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
}

void Mesh::clearVBO() {
    glDeleteBuffers(1, &m_vertexBuffer);
    m_vertexBuffer = 0;
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

void Mesh::addTriangle(unsigned int vertexIndex[3], unsigned int uvIndex[3], unsigned int normalIndex[3]) {
    if(m_vertexBuffer == 0) {
        m_vertexCount += 3;

        m_vertexIndicies.push_back(vertexIndex[0]);
        m_vertexIndicies.push_back(vertexIndex[1]);
        m_vertexIndicies.push_back(vertexIndex[2]);
        m_uvIndicies.push_back(uvIndex[0]);
        m_uvIndicies.push_back(uvIndex[1]);
        m_uvIndicies.push_back(uvIndex[2]);
        m_normalIndicies.push_back(normalIndex[0]);
        m_normalIndicies.push_back(normalIndex[1]);
        m_normalIndicies.push_back(normalIndex[2]);
    }
}

void Mesh::finalize() {
    qDebug() << "Finalizing Model:" << m_vertexCount << " vertices";
    float *vertices = new float[3*m_vertexCount];
    float *uvs = new float[2*m_vertexCount];
    float *normals = new float[3*m_vertexCount];

    int vertexIndex;
    int uvIndex;
    int normalIndex;

    for(int i=0; i<m_vertexCount; i++) {
        vertexIndex = m_vertexIndicies[i];
        uvIndex = m_uvIndicies[i];
        normalIndex = m_normalIndicies[i];

        vertices[(3*i)] = m_vertexData[vertexIndex]->x();
        vertices[(3*i)+1] = m_vertexData[vertexIndex]->y();
        vertices[(3*i)+2] = m_vertexData[vertexIndex]->z();

        uvs[(2*i)] = m_uvData[uvIndex]->x();
        uvs[(2*i)+1] = m_uvData[uvIndex]->y();

        normals[(3*i)] = m_normalData[normalIndex]->x();
        normals[(3*i)+1] = m_normalData[normalIndex]->y();
        normals[(3*i)+2] = m_normalData[normalIndex]->z();
    }

    generateVBO(vertices, uvs, normals);

    delete vertices;
    delete uvs;
    delete normals;
}

void Mesh::generateVBO(float* vertices, float* uvs, float* normals) {
    if(m_vertexBuffer == 0) {
        qDebug() << "Creating VBO pointer";
        glGenBuffers(1, &m_vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    } else {
        qDebug() << "Freeing VBO pointer";
        glDeleteBuffers(1, &m_vertexBuffer);
        glGenBuffers(1, &m_vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    }
    glBufferData(GL_ARRAY_BUFFER, 8*m_vertexCount*sizeof(float), 0, GL_DYNAMIC_DRAW); //GL_STATIC_DRAW
    glBufferSubData(GL_ARRAY_BUFFER, 0, 3*m_vertexCount*sizeof(float), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, 3*m_vertexCount*sizeof(float), 2*m_vertexCount*sizeof(float), uvs);
    glBufferSubData(GL_ARRAY_BUFFER, 5*m_vertexCount*sizeof(float), 3*m_vertexCount*sizeof(float), normals);
}

/* ===================================================
 * =============== Post Initialization ===============
 * ===================================================
 */

void Mesh::draw()
{
    m_material->beginGL();

    if(m_vertexBuffer != 0) {
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);

        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_VERTEX_ARRAY);

        glNormalPointer(GL_FLOAT, 0, (void*)(5*m_vertexCount*sizeof(float)) );
        glTexCoordPointer(2, GL_FLOAT, 0, (void*)(3*m_vertexCount*sizeof(float)) );
        glVertexPointer(3, GL_FLOAT, 0, 0);

        glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    m_material->endGL();
}
