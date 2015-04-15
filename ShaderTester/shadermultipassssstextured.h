#ifndef SHADERMULTIPASSSSSTEXTURED_H
#define SHADERMULTIPASSSSSTEXTURED_H

#include "shadermultipasssss.h"

class ShaderMultipassSSSTextured : public ShaderMultipassSSS
{
public:
    ShaderMultipassSSSTextured(QString vshader, QString fshader);
    ~ShaderMultipassSSSTextured();

    GLint m_uniform_diffuse;
    GLint m_uniform_normal;
    GLint m_uniform_specular;

    void init();
    void bindAttributes(int pass);
    void updateAttributes(int pass);
};

#endif // SHADERMULTIPASSSSSTEXTURED_H
