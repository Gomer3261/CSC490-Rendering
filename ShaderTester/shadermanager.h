#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include "shader.h"
#include "shadersimplesss.h"

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

    int setShader(int illumination_model);
};

#endif // SHADERMANAGER_H
