#ifndef POSTPROCESSSHADER_H
#define POSTPROCESSSHADER_H

#include "qgl.h"
#include <QDebug>

#include "ShaderLoader.h"

class ShaderPostProcess
{   
public:
    ShaderPostProcess(QString vshader, QString fshader, bool draw_depth);
    ShaderPostProcess(QString vshader, QString fshader, int passes, bool draw_depth);
    ShaderPostProcess(int screen_width, int screen_height, QString vshader, QString fshader, bool draw_depth);
    void resizeGL(int screen_width, int screen_height);
    GLuint getFrameBuffer();
    void paintGL();
    ~ShaderPostProcess();
protected:
    virtual GLuint bindAttributes(int pass);
    virtual void updateAttributes(int pass);

    int m_passes;
    GLuint *m_fbos;
    GLuint *m_fbo_textures;
    GLuint *m_fbo_depths;
    int m_fbo_height;
    int m_fbo_width;
    bool m_draws_depth;

    GLuint m_vbo_fbo_vertices;

    GLuint *m_program_postproc;
    GLint *m_attribute_v_coord_postproc;
    GLint *m_uniform_fbo_width;
    GLint *m_uniform_fbo_height;
    GLint *m_uniform_fbo_render_texture;
    GLint *m_uniform_fbo_depth_texture;
private:
    void init(QString vshader, QString fshader);
    void setupFBO(int pass);
    GLuint setupShaders(QString vshader, QString fshader, int pass);
};

#endif // POSTPROCESSSHADER_H
