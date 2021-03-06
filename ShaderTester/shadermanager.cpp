#include "shadermanager.h"

ShaderManager::ShaderManager() :
    m_basic_shader(new Shader("../../../../shaders/Basic.vsh", "../../../../shaders/Basic.fsh")),
    m_simple_sss_shader(new ShaderSimpleSSS("../../../../shaders/Basic.vsh", "../../../../shaders/SSS.fsh")),
    m_multipass_sss_shader(new ShaderMultipassSSS("../../../../shaders/Basic.vsh", "../../../../shaders/SSSFinal.fsh")),
    m_texture_shader(new TextureShader("../../../../shaders/Textured.vsh", "../../../../shaders/Textured.fsh")),
    m_multipass_sss_textured_shader(new ShaderMultipassSSSTextured("../../../../shaders/Textured.vsh", "../../../../shaders/SSSFinalTextured.fsh")),
    m_toon_shader(new Shader("../../../../shaders/Basic.vsh", "../../../../shaders/Toon.fsh"))
{
}

ShaderManager::~ShaderManager()
{
    delete m_basic_shader;
    delete m_simple_sss_shader;
    delete m_multipass_sss_shader;
    delete m_texture_shader;
    delete m_multipass_sss_textured_shader;
    delete m_toon_shader;
}

Shader* ShaderManager::getShader(int illumination_model)
{
    switch(illumination_model)
    {
    case 9:
        m_toon_shader->setLightingFlags(USE_AMBIENT | USE_DIFFUSE);
        return m_toon_shader;
    case 8:
        m_multipass_sss_textured_shader->setLightingFlags(USE_BASIC_LIGHTING);
        return m_multipass_sss_textured_shader;
    case 7: // Ambient, Diffuse, Specular
        m_texture_shader->setLightingFlags(USE_BASIC_LIGHTING);
        return m_texture_shader;
    case 6: // Ambient, Diffuse, Emission
        m_basic_shader->setLightingFlags(USE_AMBIENT | USE_DIFFUSE | USE_EMISSION);
        return m_basic_shader;
    case 5: // Multipass SSS, measured depth and normals for backface.
        m_multipass_sss_shader->setLightingFlags(USE_BASIC_LIGHTING);
        return m_multipass_sss_shader;
    case 4: // Approximated SSS, hardcoded depth component
        m_simple_sss_shader->setLightingFlags(USE_BASIC_LIGHTING);
        return m_simple_sss_shader;
    case 3: // Ambient, Diffuse, Specular, Emission
        m_basic_shader->setLightingFlags(USE_BASIC_LIGHTING | USE_EMISSION);
        return m_basic_shader;
    case 2: // Ambient, Diffuse, Specular
        m_basic_shader->setLightingFlags(USE_BASIC_LIGHTING);
        return m_basic_shader;
    case 1: // Ambient, Diffuse
        m_basic_shader->setLightingFlags(USE_AMBIENT | USE_DIFFUSE);
        return m_basic_shader;
    case 0: // Diffuse
    default:
        m_basic_shader->setLightingFlags(USE_DIFFUSE);
        return m_basic_shader;
    }
}

void ShaderManager::initializeGL()
{
    m_basic_shader->initializeGL();
    m_simple_sss_shader->initializeGL();
    m_multipass_sss_shader->initializeGL();
    m_texture_shader->initializeGL();
    m_multipass_sss_textured_shader->initializeGL();
    m_toon_shader->initializeGL();
}

void ShaderManager::resizeGL(int screen_width, int screen_height)
{
    m_basic_shader->resizeGL(screen_width, screen_height);
    m_simple_sss_shader->resizeGL(screen_width, screen_height);
    m_multipass_sss_shader->resizeGL(screen_width, screen_height);
    m_texture_shader->resizeGL(screen_width, screen_height);
    m_multipass_sss_textured_shader->resizeGL(screen_width, screen_height);
    m_toon_shader->resizeGL(screen_width, screen_height);
}

GLuint ShaderManager::getGlowTexture() {
    return m_basic_shader->getEmissionTexture();
}

GLuint ShaderManager::getBackfaceTexture() {
    return m_multipass_sss_shader->getBackfaceTexture();
}

GLuint ShaderManager::getBackfaceDepth() {
    return m_multipass_sss_shader->getBackfaceDepth();
}

