#include "shadermanager.h"

ShaderManager::ShaderManager() :
    m_basic_shader(new Shader("../../../../shaders/Basic.vsh", "../../../../shaders/Basic.fsh"))
{
}

ShaderManager::~ShaderManager()
{
    delete m_basic_shader;
}

int ShaderManager::setShader(int illumination_model)
{
    switch(illumination_model)
    {
    case 3: // Ambient, Diffuse, Specular, Emission
        m_basic_shader->setLightingFlags(USE_BASIC_LIGHTING | USE_EMISSION);
        m_basic_shader->beginGL();
        break;
    case 2: // Ambient, Diffuse, Specular
        m_basic_shader->setLightingFlags(USE_BASIC_LIGHTING);
        m_basic_shader->beginGL();
        break;
    case 1: // Ambient, Diffuse
        m_basic_shader->setLightingFlags(USE_AMBIENT | USE_DIFFUSE);
        m_basic_shader->beginGL();
        break;
    case 0: // Diffuse
    default:
        m_basic_shader->setLightingFlags(USE_DIFFUSE);
        m_basic_shader->beginGL();
        break;
    }
    return 1;
}

