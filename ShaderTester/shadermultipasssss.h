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

    GLuint getBackfaceTexture() { return m_fbo_textures[m_passes-2]; }
    GLuint getBackfaceDepth() { return m_fbo_depths[m_passes-2]; }
protected:
    GLuint *m_fbos;
    GLuint *m_fbo_textures;
    GLuint *m_fbo_depths;

    float m_rim_multiplier;

    GLint m_uniform_backface_width;
    GLint m_uniform_backface_height;
    GLint m_uniform_backface_texture;
    GLint m_uniform_backface_depth;
    GLint *m_uniform_exctinction_coefficient;
    GLint *m_uniform_rim_multiplier;

    void init(QString vshader, QString fshader);
    void setupFBO(int pass);
    void bindAttributes(int pass);
    void updateAttributes(int pass);

    GLint m_result_fbo;
};

#endif // SHADERMULTIPASSSSS_H
