#ifndef TEXTURESHADER_H
#define TEXTURESHADER_H

#include "shader.h"

class TextureShader : public Shader
{
public:
    TextureShader(QString vshader, QString fshader);
    ~TextureShader();

    GLint *m_uniform_diffuse;
    GLint *m_uniform_normal;
    GLint *m_uniform_specular;

    void init();
    void bindAttributes(int pass);
    void updateAttributes(int pass);
};

#endif // TEXTURESHADER_H
