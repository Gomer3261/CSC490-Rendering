#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include "shader.h"
#include "shadersimplesss.h"
#include "shadermultipasssss.h"
#include "shadermultipassssstextured.h"
#include "textureshader.h"

class ShaderManager
{
private:
    // Private Constructor
    ShaderManager();
    // Stop the compiler generating methods of copy the object
    ShaderManager(ShaderManager const& copy);            // Not Implemented
    ShaderManager& operator=(ShaderManager const& copy); // Not Implemented

    Shader *m_basic_shader;
    ShaderSimpleSSS *m_simple_sss_shader;
    ShaderMultipassSSS *m_multipass_sss_shader;
    TextureShader *m_texture_shader;
    ShaderMultipassSSSTextured *m_multipass_sss_textured_shader;
    Shader *m_toon_shader;
public:
    static ShaderManager& getInstance()
    {
        // The only instance
        // Guaranteed to be lazy initialized
        // Guaranteed that it will be destroyed correctly
        static ShaderManager instance;
        return instance;
    }

    ~ShaderManager();

    Shader* getShader(int illumination_model);
    void initializeGL();
    void resizeGL(int screen_width, int screen_height);

    // Shader data accessors.
    GLuint getGlowTexture();
    GLuint getBackfaceTexture();
    GLuint getBackfaceDepth();
};

#endif // SHADERMANAGER_H
