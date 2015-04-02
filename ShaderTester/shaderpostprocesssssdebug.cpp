#include "shaderpostprocesssssdebug.h"

ShaderPostProcessSSSDebug::ShaderPostProcessSSSDebug(QString vshader, QString fshader, bool draw_depth) :
    ShaderPostProcess(vshader, fshader, 1, draw_depth),
    m_uniform_backface_input(new GLint[1])
{
    init();
}

void ShaderPostProcessSSSDebug::init()
{
    // Cannot call function from inheriting class in constructor. Each class must bind it's own attributes.
    for(int i=0; i<m_passes; i++) {
        bindAttributes(i);
    }
}

GLuint ShaderPostProcessSSSDebug::bindAttributes(int pass)
{
    m_uniform_backface_input[pass] = glGetUniformLocation(m_program_postproc[pass], "fbo_backface");
    if (m_uniform_backface_input[pass] == -1) {
        qWarning() << "Could not bind uniform fbo_backface";
        return 0;
    }

    return 1;
}

void ShaderPostProcessSSSDebug::updateAttributes(int pass)
{
    ShaderPostProcess::updateAttributes(pass);
    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_2D, ShaderManager::getInstance().getBackfaceTexture());

    glUniform1i(m_uniform_backface_input[pass], 2);
}

