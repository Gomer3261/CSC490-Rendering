#ifndef POSTPROCESSSHADER_H
#define POSTPROCESSSHADER_H

#include "qgl.h"
#include <QDebug>

#include "ShaderLoader.h"

class ShaderPostProcess
{   
public:
    ShaderPostProcess(int screen_width, int screen_height, QString vshader, QString fshader);
    void resizeGL(int screen_width, int screen_height);
    GLuint getFrameBuffer();
    void paintGL();
    ~ShaderPostProcess();
protected:
    GLuint setupShader(QString vshader, QString fshader);
    virtual GLuint bindAttributes();
    virtual void updateAttributes();

    GLuint m_vbo_fbo_vertices;
    GLuint m_fbo;
    int m_fbo_height;
    int m_fbo_width;
    GLuint m_fbo_texture;
    GLuint m_fbo_depth;

    GLuint m_program_postproc;
    GLuint m_attribute_v_coord_postproc;
    GLuint m_uniform_fbo_width;
    GLuint m_uniform_fbo_height;
    GLuint m_uniform_fbo_render_texture;
    GLuint m_uniform_fbo_depth_texture;
};

#endif // POSTPROCESSSHADER_H
