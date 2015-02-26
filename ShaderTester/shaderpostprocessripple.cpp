#include "shaderpostprocessripple.h"

ShaderPostProcessRipple::ShaderPostProcessRipple(int screen_width, int screen_height, QString vshader, QString fshader) :
    ShaderPostProcess(screen_width, screen_height, vshader, fshader),
    m_timer()
{
    // Cannot call function from inheriting class in constructor. Each class must bind it's own attributes.
    bindAttributes();
    m_timer.start();
}

GLuint ShaderPostProcessRipple::bindAttributes()
{
    m_uniform_offset = glGetUniformLocation(m_program_postproc, "offset");
    if (m_uniform_offset == -1) {
        qWarning() << "Could not bind uniform offset";
        return 0;
    }

    return 1;
}

void ShaderPostProcessRipple::updateAttributes()
{
    ShaderPostProcess::updateAttributes();

    glUniform1f(m_uniform_offset, float(m_timer.elapsed() * 2*3.14159 * .75 / 1000));
}

