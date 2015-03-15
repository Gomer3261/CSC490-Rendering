#ifndef SHADERSIMPLESSS_H
#define SHADERSIMPLESSS_H

#include "shader.h"

class ShaderSimpleSSS : public Shader
{
public:
    ShaderSimpleSSS(QString vshader, QString fshader);
    ~ShaderSimpleSSS();
protected:
    float m_thickness;
    float m_rim_multiplier;

    GLint m_uniform_thickness;
    GLint m_uniform_exctinction_coefficient;
    GLint m_uniform_rim_multiplier;

    void init();
    void bindAttributes();
    void updateAttributes();
};

#endif // SHADERSIMPLESSS_H
