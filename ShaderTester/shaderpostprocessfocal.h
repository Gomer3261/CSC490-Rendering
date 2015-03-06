#ifndef SHADERPOSTPROCESSFOCAL_H
#define SHADERPOSTPROCESSFOCAL_H

#include "shaderpostprocess.h"

#include <QElapsedTimer>

class ShaderPostProcessFocal : public ShaderPostProcess
{
public:
    ShaderPostProcessFocal(QString vshader, QString fshader, bool draw_depth);
    ShaderPostProcessFocal(QString vshader, QString fshader, int passes, bool draw_depth);
    ShaderPostProcessFocal(int screen_width, int screen_height, QString vshader, QString fshader, bool draw_depth);
protected:
    GLuint bindAttributes(int pass);
    void updateAttributes(int pass);

    float *m_focal_point;
    int m_focal_point_index;
    GLint *m_uniform_focal_point;
private:
    void init();
};

#endif // SHADERPOSTPROCESSFOCAL_H
