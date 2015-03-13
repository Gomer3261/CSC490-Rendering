#include "shader.h"

Shader::Shader(QString vshader, QString fshader) :
    m_program(0),
    m_lighting_flags(USE_BASIC_LIGHTING),
    m_uniform_lcount(0),
    m_uniform_ambient(0),
    m_uniform_diffuse(0),
    m_uniform_specular(0),
    m_uniform_emission(0)
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
    updateAttributes();
}

void Shader::bindAttributes()
{
    m_uniform_lcount = glGetUniformLocation(m_program, "lCount");
    if (m_uniform_lcount == -1) {
        qWarning() << "Could not bind uniform lCount";
    }

    m_uniform_ambient = glGetUniformLocation(m_program, "ambient_on");
    if (m_uniform_ambient == -1) {
        qWarning() << "Could not bind uniform ambient_on";
    }

    m_uniform_diffuse = glGetUniformLocation(m_program, "diffuse_on");
    if (m_uniform_diffuse == -1) {
        qWarning() << "Could not bind uniform diffuse_on";
    }

    m_uniform_specular = glGetUniformLocation(m_program, "specular_on");
    if (m_uniform_specular == -1) {
        qWarning() << "Could not bind uniform specular_on";
    }

    m_uniform_emission = glGetUniformLocation(m_program, "emission_on");
    if (m_uniform_emission == -1) {
        qWarning() << "Could not bind uniform emission_on";
    }
}

void Shader::updateAttributes()
{
    glUniform1i(m_uniform_lcount, 2);
    glUniform1i(m_uniform_ambient, (m_lighting_flags & USE_AMBIENT) > 0);
    glUniform1i(m_uniform_diffuse, (m_lighting_flags & USE_DIFFUSE) > 0);
    glUniform1i(m_uniform_specular, (m_lighting_flags & USE_SPECULAR) > 0);
    glUniform1i(m_uniform_emission, (m_lighting_flags & USE_EMISSION) > 0);
}

void Shader::loadShader(QString vshader, QString fshader)
{
    m_program = ShaderLoader::loadProgram(vshader, fshader);
    bindAttributes();
}

