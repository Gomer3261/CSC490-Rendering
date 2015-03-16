#ifndef SCENE_H
#define SCENE_H

#include "light.h"
#include "orbitallight.h"
#include "renderobject.h"
#include "shadermanager.h"
#include "shaderpostprocess.h"

#include <QElapsedTimer>

class Scene
{
public:
    Scene();
    ~Scene();
    void addObject(RenderObject* object);
    void addFilter(ShaderPostProcess* filter);

    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

    int getLightCount();
private:
    QList<Light*> m_lights;
    QList<RenderObject*> m_objects;
    QList<ShaderPostProcess*> m_filters;
};

#endif // SCENE_H
