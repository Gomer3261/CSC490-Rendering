#include "shadersimplesss.h"

ShaderSimpleSSS::ShaderSimpleSSS(QString vshader, QString fshader) :
    Shader(vshader, fshader),
    m_thickness(0.6f),
    m_rim_multiplier(0.4f),
    m_uniform_thickness(0),
    m_uniform_exctinction_coefficient(0),
    m_uniform_rim_multiplier(0)
{
    init();
}

ShaderSimpleSSS::~ShaderSimpleSSS()
{

}

void ShaderSimpleSSS::init()
{
    bindAttributes();
}

void ShaderSimpleSSS::bindAttributes()
{
    m_uniform_thickness = glGetUniformLocation(m_program, "thickness");
    if (m_uniform_thickness == -1) {
        qWarning() << "Could not bind uniform thickness";
    }

    m_uniform_exctinction_coefficient = glGetUniformLocation(m_program, "exctinction_coefficient");
    if (m_uniform_exctinction_coefficient == -1) {
        qWarning() << "Could not bind uniform exctinction_coefficient";
    }

    m_uniform_rim_multiplier = glGetUniformLocation(m_program, "rim_multiplier");
    if (m_uniform_rim_multiplier == -1) {
        qWarning() << "Could not bind uniform rim_multiplier";
    }
}

void ShaderSimpleSSS::updateAttributes()
{
    Shader::updateAttributes();

    glUniform1f(m_uniform_thickness, m_thickness);
    glUniform3f(m_uniform_exctinction_coefficient, 0.7f, 0.1f, 0.4f);
    glUniform1f(m_uniform_rim_multiplier, m_rim_multiplier);
}
