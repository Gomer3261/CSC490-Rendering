#ifndef SHADERPOSTPROCESSRIPPLE_H
#define SHADERPOSTPROCESSRIPPLE_H

#include "shaderpostprocess.h"

#include <QElapsedTimer>

class ShaderPostProcessRipple : public ShaderPostProcess
{
public:
    ShaderPostProcessRipple(int screen_width, int screen_height, QString vshader, QString fshader);
protected:
    GLuint bindAttributes();
    void updateAttributes();

    QElapsedTimer m_timer;
    GLuint m_uniform_offset;
};

#endif // SHADERPOSTPROCESSRIPPLE_H
