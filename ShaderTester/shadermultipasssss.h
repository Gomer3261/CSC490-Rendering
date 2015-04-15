#ifndef SHADERMULTIPASSSSS_H
#define SHADERMULTIPASSSSS_H

#include "shader.h"

/**
 * @brief The ShaderMultipassSSS class
 *
 * Shader split into 4 passes.
 * First is backface pass rendering backface depth and normals
 * Second is pure lighting pass rendering on frontface.
 * Third blurs vertically as a post process effect
 * Fourth is frontface render that blurs horizontally and applies to result.
 */
class ShaderMultipassSSS : public Shader
{
public:
    ShaderMultipassSSS(QString vshader, QString fshader);
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

    GLint *m_uniform_fbo_width;
    GLint *m_uniform_fbo_height;
    GLint *m_uniform_fbo_texture;
    GLint *m_uniform_fbo_depth;

    GLint m_uniform_rim_multiplier;

    void init();
    void setupFBO(int pass);
    void bindAttributes(int pass);
    void updateAttributes(int pass);

    GLint m_result_fbo;
};

#endif // SHADERMULTIPASSSSS_H
