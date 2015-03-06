#include "shaderpostprocessfocal.h"

#define DEPTH_SAMPLES 32

ShaderPostProcessFocal::ShaderPostProcessFocal(QString vshader, QString fshader, bool draw_depth) :
    ShaderPostProcess(vshader, fshader, draw_depth),
    m_focal_point(new float[DEPTH_SAMPLES]),
    m_focal_point_index(0),
    m_uniform_focal_point(new GLint[1])
{
    init();
}

ShaderPostProcessFocal::ShaderPostProcessFocal(QString vshader, QString fshader, int passes, bool draw_depth) :
    ShaderPostProcess(vshader, fshader, passes, draw_depth),
    m_focal_point(new float[DEPTH_SAMPLES]),
    m_focal_point_index(0),
    m_uniform_focal_point(new GLint[passes])
{
    init();
}

ShaderPostProcessFocal::ShaderPostProcessFocal(int screen_width, int screen_height, QString vshader, QString fshader, bool draw_depth) :
    ShaderPostProcess(screen_width, screen_height, vshader, fshader, draw_depth),
    m_focal_point(new float[DEPTH_SAMPLES]),
    m_focal_point_index(0),
    m_uniform_focal_point(new GLint[1])
{
    init();
}

/*ShaderPostProcessFocal::~ShaderPostProcessFocal() {

}*/

void ShaderPostProcessFocal::init()
{
    // Cannot call function from inheriting class in constructor. Each class must bind it's own attributes.
    for(int i=0; i<m_passes; i++) {
        bindAttributes(i);
    }

    for(int i=0; i<DEPTH_SAMPLES; i++) {
        m_focal_point[i] = 0.5f;
    }
}

GLuint ShaderPostProcessFocal::bindAttributes(int pass)
{
    m_uniform_focal_point[pass] = glGetUniformLocation(m_program_postproc[pass], "focal_point");
    if (m_uniform_focal_point[pass] == -1) {
        qWarning() << "Could not bind uniform focal_point";
        return 0;
    }

    return 1;
}

void ShaderPostProcessFocal::updateAttributes(int pass)
{
    //QElapsedTimer timer = QElapsedTimer();
    //timer.start();

    if(pass == 0) {
        GLuint current_depth = 0;
        glReadPixels(m_fbo_width/2, m_fbo_height/2, 1, 1, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, &current_depth);

        m_focal_point[m_focal_point_index] = (float(current_depth)/4294967295);
        m_focal_point_index = (m_focal_point_index+1)%DEPTH_SAMPLES;
    }

    float floating_focal = 0.0;
    for(int i=0; i<DEPTH_SAMPLES; i++) {
        floating_focal += m_focal_point[i];
    }
    floating_focal /= DEPTH_SAMPLES;

    //qDebug() << timer.elapsed();

    //m_focal_point = m_focal_point*0.95 + (float(current_depth)/4294967295)*0.05;

    ShaderPostProcess::updateAttributes(pass);

    glUniform1f(m_uniform_focal_point[pass], floating_focal); // MAX UINT
}

