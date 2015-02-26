#ifndef SHADER_H
#define SHADER_H

#include <QFileInfo>
#include <QDebug>

#include "shaderloader.h"

class Shader
{
public:
    Shader(QString vshader, QString fshader);
    ~Shader();
    void beginGL();
protected:
    GLuint m_program;
    GLuint m_uniform_lcount;

    void loadShader(QString vshader, QString fshader);
    void bindAttributes();
};

#endif // SHADER_H
