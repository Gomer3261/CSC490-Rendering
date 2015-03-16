#include "shaderpostprocessglow.h"

ShaderPostProcessGlow::ShaderPostProcessGlow(QString vshader, QString fshader, bool draw_depth) :
    ShaderPostProcess(vshader, fshader, 2, draw_depth),
    m_uniform_glow_input(new GLint[2])
{
    init();
}

void ShaderPostProcessGlow::init()
{
    // Cannot call function from inheriting class in constructor. Each class must bind it's own attributes.
    for(int i=0; i<m_passes; i++) {
        bindAttributes(i);
    }
}

GLuint ShaderPostProcessGlow::bindAttributes(int pass)
{
    m_uniform_glow_input[pass] = glGetUniformLocation(m_program_postproc[pass], "fbo_glow");
    if (m_uniform_glow_input[pass] == -1) {
        qWarning() << "Could not bind uniform fbo_glow";
        return 0;
    }

    return 1;
}

void ShaderPostProcessGlow::updateAttributes(int pass)
{
    ShaderPostProcess::updateAttributes(pass);

    if(pass == 0) {
        glActiveTexture(GL_TEXTURE0 + 2);
        glBindTexture(GL_TEXTURE_2D, ShaderManager::getInstance().getGlowTexture());
    } else {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_fbo_textures[0]);
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, m_fbo_depths[0]);
        glActiveTexture(GL_TEXTURE0 + 2);
        glBindTexture(GL_TEXTURE_2D, m_fbo_textures[1]);
    }

    glUniform1i(m_uniform_glow_input[pass], 2);
}
