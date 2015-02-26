#include "shader.h"

Shader::Shader(QString vshader, QString fshader) :
    m_program(0),
    m_uniform_lcount(0)
{
    loadShader(vshader, fshader);
}

Shader::~Shader()
{
    glDeleteProgram(m_program);
}

void Shader::beginGL()
{
    glUseProgram(m_program);
    glUniform1i(m_uniform_lcount, 2);
}

void Shader::bindAttributes()
{
    m_uniform_lcount = glGetUniformLocation(m_program, "lCount");
    if (m_uniform_lcount == -1) {
        qWarning() << "Could not bind uniform lCount";
        return;
    }
}

void Shader::loadShader(QString vshader, QString fshader)
{
    m_program = ShaderLoader::loadProgram(vshader, fshader);
    bindAttributes();
}

