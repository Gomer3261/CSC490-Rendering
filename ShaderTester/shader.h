#ifndef SHADER_H
#define SHADER_H

#define USE_AMBIENT             (1 << 0)
#define USE_DIFFUSE             (1 << 1)
#define USE_SPECULAR            (1 << 2)
#define USE_EMISSION            (1 << 3)
#define USE_BASIC_LIGHTING      (USE_AMBIENT | USE_DIFFUSE | USE_SPECULAR)

#include <QFileInfo>
#include <QDebug>

#include "shaderloader.h"

class Shader
{
public:


    Shader(QString vshader, QString fshader);
    virtual ~Shader();
    void beginGL();
    void setLightingFlags(int flags) { m_lighting_flags = flags; }
protected:
    GLuint m_program;

    int m_lighting_flags;

    GLint m_uniform_lcount;
    GLint m_uniform_ambient;
    GLint m_uniform_diffuse;
    GLint m_uniform_specular;
    GLint m_uniform_emission;

    void loadShader(QString vshader, QString fshader);
    virtual void bindAttributes();
    virtual void updateAttributes();
};

#endif // SHADER_H
