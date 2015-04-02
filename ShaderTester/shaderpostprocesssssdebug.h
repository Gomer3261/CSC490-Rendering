#ifndef SHADERPOSTPROCESSSSSDEBUG_H
#define SHADERPOSTPROCESSSSSDEBUG_H

#include "shaderpostprocess.h"
#include "shadermanager.h"

class ShaderPostProcessSSSDebug : public ShaderPostProcess
{
public:
    ShaderPostProcessSSSDebug(QString vshader, QString fshader, bool draw_depth);
protected:
    GLuint bindAttributes(int pass);
    void updateAttributes(int pass);

    GLint *m_uniform_backface_input;
private:
    void init();
};

#endif // SHADERPOSTPROCESSSSSDEBUG_H
