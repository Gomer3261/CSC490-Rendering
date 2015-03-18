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
    Shader(QString vshader, QString fshader, int passes);
    virtual ~Shader();
    virtual void initializeGL() {}
    virtual void beginGL(int pass);
    virtual void endGL() {}
    virtual void resizeGL(int screen_width, int screen_height);
    void setLightingFlags(int flags) { m_lighting_flags = flags; }
    int passCount() {return m_passes;}

    GLuint getEmissionTexture();
    GLuint getEmissionDepth();
protected:
    int m_lighting_flags;

    int m_passes;

    int m_screen_width;
    int m_screen_height;

    static GLuint m_emission_texture;

    GLuint *m_programs;

    GLint *m_uniform_lcount;
    GLint *m_uniform_ambient;
    GLint *m_uniform_diffuse;
    GLint *m_uniform_specular;
    GLint *m_uniform_emission;

    void init(QString vshader, QString fshader);
    virtual void bindAttributes(int pass);
    virtual void updateAttributes(int pass);
};

#endif // SHADER_H
