#ifndef SHADERMULTIPASSSSS_H
#define SHADERMULTIPASSSSS_H

#include "shader.h"

class ShaderMultipassSSS : public Shader
{
public:
    ShaderMultipassSSS(QString vshader, QString fshader, QString vshader2, QString fshader2);
    ~ShaderMultipassSSS();

    void resizeGL(int screen_width, int screen_height);
    void beginGL(int pass);
    void endGL(int pass);

    GLuint getBackfaceTexture() { return m_fbo_textures[1]; }
    GLuint getBackfaceDepth() { return m_fbo_depths[1]; }
protected:
    GLuint *m_fbos;
    GLuint *m_fbo_textures;
    GLuint *m_fbo_depths;

    GLuint m_blur_program;
    GLuint m_blur_vertices;
    GLint m_attribute_v_coord_postproc;

    float m_rim_multiplier;

    GLint m_uniform_backface_width;
    GLint m_uniform_backface_height;
    GLint m_uniform_backface_texture;
    GLint m_uniform_backface_depth;

    GLint m_uniform_blurred_backface_width;
    GLint m_uniform_blurred_backface_height;
    GLint m_uniform_blurred_backface_texture;
    GLint m_uniform_blurred_backface_depth;

    GLint m_uniform_exctinction_coefficient;
    GLint m_uniform_rim_multiplier;

    void init(QString vshader, QString fshader);
    void setupFBO(int pass);
    void bindAttributes(int pass);
    void updateAttributes(int pass);

    GLint m_result_fbo;
};

#endif // SHADERMULTIPASSSSS_H
