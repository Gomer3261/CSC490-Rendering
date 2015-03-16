#ifndef SHADERPOSTPROCESSGLOW_H
#define SHADERPOSTPROCESSGLOW_H

#include "shaderpostprocess.h"
#include "shadermanager.h"

class ShaderPostProcessGlow : public ShaderPostProcess
{
public:
    ShaderPostProcessGlow(QString vshader, QString fshader, bool draw_depth);
protected:
    GLuint bindAttributes(int pass);
    void updateAttributes(int pass);

    GLint *m_uniform_glow_input;
private:
    void init();
};

#endif // SHADERPOSTPROCESSGLOW_H
