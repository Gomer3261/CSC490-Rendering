#include "textureshader.h"

TextureShader::TextureShader(QString vshader, QString fshader) :
    Shader(vshader, fshader),
    m_uniform_diffuse(new GLint(m_passes)),
    m_uniform_normal(new GLint(m_passes)),
    m_uniform_specular(new GLint(m_passes))
{

}

TextureShader::~TextureShader()
{
    delete m_uniform_diffuse;
    delete m_uniform_normal;
    delete m_uniform_specular;
}

void TextureShader::init()
{
    for(int i=0; i<m_passes; i++) {
        bindAttributes(i);
    }
}

void TextureShader::bindAttributes(int pass)
{
    m_uniform_diffuse[pass] = glGetUniformLocation(m_programs[pass], "diffuse_texture");
    if (m_uniform_diffuse[pass] == -1) {
        qWarning() << "Could not bind uniform diffuse_texture";
    }

    m_uniform_normal[pass] = glGetUniformLocation(m_programs[pass], "normal_texture");
    if (m_uniform_normal[pass] == -1) {
        qWarning() << "Could not bind uniform normal_texture";
    }

    m_uniform_normal[pass] = glGetUniformLocation(m_programs[pass], "specular_texture");
    if (m_uniform_specular[pass] == -1) {
        qWarning() << "Could not bind uniform specular_texture";
    }
}

void TextureShader::updateAttributes(int pass)
{
    Shader::updateAttributes(pass);

    glUniform1i(m_uniform_diffuse[pass], 0);
    glUniform1i(m_uniform_normal[pass], 2);
    glUniform1i(m_uniform_specular[pass], 3);
}
