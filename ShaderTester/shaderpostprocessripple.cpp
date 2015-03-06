#include "shaderpostprocessripple.h"

ShaderPostProcessRipple::ShaderPostProcessRipple(QString vshader, QString fshader, bool draw_depth) :
    ShaderPostProcess(vshader, fshader, draw_depth),
    m_timer(),
    m_uniform_offset(new GLint[1])
{
    init();
}

ShaderPostProcessRipple::ShaderPostProcessRipple(QString vshader, QString fshader, int passes, bool draw_depth) :
    ShaderPostProcess(vshader, fshader, passes, draw_depth),
    m_timer(),
    m_uniform_offset(new GLint[passes])
{
    init();
}

ShaderPostProcessRipple::ShaderPostProcessRipple(int screen_width, int screen_height, QString vshader, QString fshader, bool draw_depth) :
    ShaderPostProcess(screen_width, screen_height, vshader, fshader, draw_depth),
    m_timer(),
    m_uniform_offset(new GLint[1])
{
    init();
}

void ShaderPostProcessRipple::init()
{
    // Cannot call function from inheriting class in constructor. Each class must bind it's own attributes.
    for(int i=0; i<m_passes; i++) {
        bindAttributes(i);
    }
    m_timer.start();
}

GLuint ShaderPostProcessRipple::bindAttributes(int pass)
{
    m_uniform_offset[pass] = glGetUniformLocation(m_program_postproc[pass], "offset");
    if (m_uniform_offset[pass] == -1) {
        qWarning() << "Could not bind uniform offset";
        return 0;
    }

    return 1;
}

void ShaderPostProcessRipple::updateAttributes(int pass)
{
    ShaderPostProcess::updateAttributes(pass);

    glUniform1f(m_uniform_offset[pass], float(m_timer.elapsed() * 2*3.14159 * .75 / 1000));
}

