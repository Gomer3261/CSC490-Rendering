#ifndef SHADERLOADER_H
#define SHADERLOADER_H

#include "qgl.h"
#include <QDebug>
#include <QFileInfo>

class ShaderLoader
{
public:
    static GLuint loadShader(QString shader_file, GLuint type);
    static GLuint loadProgram(QString vshader, QString fshader);
    static void print_log(GLuint object);
};

#endif // SHADERLOADER_H
