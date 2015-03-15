#ifndef SHADERTEXTUREOUTPUT_H
#define SHADERTEXTUREOUTPUT_H

#include "shader.h"

class ShaderTextureOutput : public Shader
{
public:
    ShaderTextureOutput(QString vshader, QString fshader);
    ~ShaderTextureOutput();
    void resizeGL(int screen_width, int screen_height);

    GLuint getTexture();
protected:
    int m_screen_width;
    int m_screen_height;

    GLuint m_output_texture;

    GLint m_uniform_texture_width;
    GLint m_uniform_texture_height;
    GLint m_uniform_output_texture;

    void init();
    void bindAttributes();
    void updateAttributes();
};

#endif // SHADERTEXTUREOUTPUT_H
