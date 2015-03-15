#include "shadertextureoutput.h"

ShaderTextureOutput::ShaderTextureOutput(QString vshader, QString fshader) :
    Shader(vshader, fshader),
    m_screen_width(256),
    m_screen_height(256),
    m_output_texture(0),
    m_uniform_output_texture(0)
{
    init();
}

ShaderTextureOutput::~ShaderTextureOutput()
{
    glDeleteTextures(1, &m_output_texture);
}

GLuint ShaderTextureOutput::getTexture() {
    return m_output_texture;
}

void ShaderTextureOutput::init() {
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &m_output_texture);
    glBindTexture(GL_TEXTURE_2D, m_output_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_screen_width, m_screen_height, 0, GL_RGBA,
        GL_UNSIGNED_BYTE, NULL);

    bindAttributes();
}

void ShaderTextureOutput::resizeGL(int screen_width, int screen_height) {
    m_screen_width = screen_width;
    m_screen_height = screen_height;

    //TODO: Resize texture.
}

void ShaderTextureOutput::bindAttributes()
{
    m_uniform_texture_width = glGetUniformLocation(m_program, "texture_width");
    if (m_uniform_texture_width == -1) {
        qWarning() << "Could not bind uniform texture_width";
    }

    m_uniform_texture_height = glGetUniformLocation(m_program, "texture_height");
    if (m_uniform_texture_height == -1) {
        qWarning() << "Could not bind uniform texture_height";
    }

    m_uniform_output_texture = glGetUniformLocation(m_program, "output_texture");
    if (m_uniform_output_texture == -1) {
        qWarning() << "Could not bind uniform output_texture";
    }
}

void ShaderTextureOutput::updateAttributes()
{
    Shader::updateAttributes();

    glUniform1i(m_uniform_texture_width, m_screen_width);
    glUniform1i(m_uniform_texture_height, m_screen_height);
    glUniform1i(m_uniform_output_texture, m_output_texture);
}
