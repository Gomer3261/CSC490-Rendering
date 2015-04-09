#include "shadersimplesss.h"

ShaderSimpleSSS::ShaderSimpleSSS(QString vshader, QString fshader) :
    Shader(vshader, fshader),
    m_thickness(0.6f),
    m_rim_multiplier(0.4f),
    m_uniform_thickness(new GLint[m_passes]),
    m_uniform_rim_multiplier(new GLint[m_passes])
{
    init();
}

ShaderSimpleSSS::~ShaderSimpleSSS()
{
    delete m_uniform_thickness;
    delete m_uniform_rim_multiplier;
}

void ShaderSimpleSSS::init()
{
    for(int i=0; i<m_passes; i++) {
        bindAttributes(i);
    }
}

void ShaderSimpleSSS::bindAttributes(int pass)
{
    m_uniform_thickness[pass] = glGetUniformLocation(m_programs[pass], "thickness");
    if (m_uniform_thickness[pass] == -1) {
        qWarning() << "Could not bind uniform thickness";
    }

    m_uniform_rim_multiplier[pass] = glGetUniformLocation(m_programs[pass], "rim_multiplier");
    if (m_uniform_rim_multiplier[pass] == -1) {
        qWarning() << "Could not bind uniform rim_multiplier";
    }
}

void ShaderSimpleSSS::updateAttributes(int pass)
{
    Shader::updateAttributes(pass);

    glUniform1f(m_uniform_thickness[pass], m_thickness);
    glUniform1f(m_uniform_rim_multiplier[pass], m_rim_multiplier);
}
