#include "scene.h"

Scene::Scene() :
    m_lights(),
    m_objects(),
    m_filters(),
    m_screen_width(0),
    m_screen_height(0)
{
    QMatrix4x4 lightTransform;
    lightTransform.rotate(-90, 1, 0, 0);
    OrbitalLight* light_1 = new OrbitalLight(GL_LIGHT0, lightTransform, 10.0f, 5.0f, 0.01f);

    GLfloat lightSpec[] = {0.4f, 0.4f, 1.0f, 1.0f};
    GLfloat lightDif[] = {0.2f, 0.2f, 0.7f, 1.0f};
    GLfloat lightAmb[] = {0.05f, 0.05f, 0.25f, 1.0f};

    light_1->initializeGL(lightSpec, lightDif, lightAmb);

    m_lights.append(light_1);

    lightTransform.rotate(180, 0, 0, 1);

    OrbitalLight* light_2 = new OrbitalLight(GL_LIGHT1, lightTransform, 10.0f, 5.0f, 0.01f);

    GLfloat lightSpec_2[] = {1.0f, 0.8f, 0.0f, 1.0f};
    GLfloat lightDif_2[] = {0.7f, 0.5f, 0.0f, 1.0f};
    GLfloat lightAmb_2[] = {0.25f, 0.20f, 0.0f, 1.0f};

    light_2->initializeGL(lightSpec_2, lightDif_2, lightAmb_2);

    m_lights.append(light_2);
}

Scene::~Scene()
{
    for(int i=0; i<m_lights.length(); i++) {
        delete m_lights[i];
    }
    m_lights.clear();
    for(int i=0; i<m_objects.length(); i++) {
        delete m_objects[i];
    }
    m_objects.clear();
}

void Scene::addObject(RenderObject* object) {
    m_objects.append(object);
}

void Scene::addFilter(ShaderPostProcess* filter) {
    m_filters.append(filter);
}

void Scene::initializeGL()
{
    /*for(int i=0; i<m_lights.length(); i++) {
        m_lights[i]->initializeGL();
    }*/

    for(int i=0; i<m_objects.size(); i++) {
        m_objects[i]->initializeGL();
    }
}

void Scene::resizeGL(int width, int height) {
    m_screen_width = width;
    m_screen_height = height;
    for(ShaderPostProcess* filter : m_filters) {
        filter->resizeGL(width, height);
    }
}

void Scene::paintGL()
{
    QElapsedTimer timer;
    timer.start();

    for(int render_pass=0; render_pass<m_filters.length()+1; render_pass++)
    {
        if(render_pass < m_filters.length())
        {
            glBindFramebuffer(GL_FRAMEBUFFER, m_filters[render_pass]->getFrameBuffer());
        }
        else
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        if( render_pass > 0 ) {
            glBindFramebuffer(GL_READ_FRAMEBUFFER, m_filters[render_pass-1]->getFrameBuffer());
        }

        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if(render_pass == 0) {
            for(int i=0; i<m_lights.length(); i++) {
                m_lights[i]->paintGL();
            }

            glEnable(GL_LIGHTING);
            for(int i=0; i<m_lights.size(); i++) {
                glEnable(m_lights[i]->getLightId());
            }

            for(int i=0; i<m_objects.size(); i++) {
                m_objects[i]->paintGL();
            }

            for(int i=0; i<m_lights.size(); i++) {
                glDisable(m_lights[i]->getLightId());
            }
            glDisable(GL_LIGHTING);
        } else {
            m_filters[render_pass-1]->paintGL();
            glBlitFramebuffer(0, 0, m_screen_width, m_screen_height, 0, 0, m_screen_width, m_screen_height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
        }
    }

    //qDebug() << "Draw time: " << timer.nsecsElapsed()/1000000.0f << "µs";
}

int Scene::getLightCount()
{
    return m_lights.length();
}
