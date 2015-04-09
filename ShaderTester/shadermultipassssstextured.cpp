#include "shadermultipassssstextured.h"

ShaderMultipassSSSTextured::ShaderMultipassSSSTextured(QString vshader, QString fshader, QString vshader2, QString fshader2) :
    ShaderMultipassSSS(vshader, fshader, vshader2, fshader2),
    m_uniform_diffuse(0),
    m_uniform_normal(0),
    m_uniform_specular(0)
{
    init();
}

ShaderMultipassSSSTextured::~ShaderMultipassSSSTextured()
{
}

void ShaderMultipassSSSTextured::init()
{
    for(int i=0; i<m_passes; i++) {
        bindAttributes(i);
    }
}

void ShaderMultipassSSSTextured::bindAttributes(int pass)
{
    if(pass == 1) {
        m_uniform_diffuse = glGetUniformLocation(m_programs[pass], "diffuse_texture");
        if (m_uniform_diffuse == -1) {
            qWarning() << "Could not bind uniform diffuse_texture";
        }

        m_uniform_normal = glGetUniformLocation(m_programs[pass], "normal_texture");
        if (m_uniform_normal == -1) {
            qWarning() << "Could not bind uniform normal_texture";
        }

        m_uniform_specular = glGetUniformLocation(m_programs[pass], "specular_texture");
        if (m_uniform_specular == -1) {
            qWarning() << "Could not bind uniform specular_texture";
        }
    }
}

void ShaderMultipassSSSTextured::updateAttributes(int pass)
{
    ShaderMultipassSSS::updateAttributes(pass);

    if(pass == 1) {
        glUniform1i(m_uniform_diffuse, 0);
        glUniform1i(m_uniform_normal, 2);
        glUniform1i(m_uniform_specular, 3);
    }
}
