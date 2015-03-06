#ifndef SHADERPOSTPROCESSRIPPLE_H
#define SHADERPOSTPROCESSRIPPLE_H

#include "shaderpostprocess.h"

#include <QElapsedTimer>

class ShaderPostProcessRipple : public ShaderPostProcess
{
public:
    ShaderPostProcessRipple(QString vshader, QString fshader, bool draw_depth);
    ShaderPostProcessRipple(QString vshader, QString fshader, int passes, bool draw_depth);
    ShaderPostProcessRipple(int screen_width, int screen_height, QString vshader, QString fshader, bool draw_depth);
protected:
    GLuint bindAttributes(int pass);
    void updateAttributes(int pass);

    QElapsedTimer m_timer;
    GLint *m_uniform_offset;
private:
    void init();
};

#endif // SHADERPOSTPROCESSRIPPLE_H
