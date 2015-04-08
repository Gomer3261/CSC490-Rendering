#include "scene.h"

Scene::Scene() :
    m_lights(),
    m_objects(),
    m_filters()
{
    QMatrix4x4 lightTransform;
    lightTransform.rotate(-90, 1, 0, 0);
    OrbitalLight* light_1 = new OrbitalLight(GL_LIGHT0, lightTransform, 10.0f, 5.0f, 0.01f);

    //GLfloat lightSpec[] = {0.4f, 0.4f, 1.0f, 1.0f};
    GLfloat lightSpec[] = {0.6f, 0.6f, 0.6f, 1.0f};
    GLfloat lightDif[] = {0.6f, 0.6f, 0.9f, 1.0f};
    //GLfloat lightDif[] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat lightAmb[] = {0.005f, 0.005f, 0.025f, 1.0f};

    light_1->initializeGL(lightSpec, lightDif, lightAmb);

    m_lights.append(light_1);

    lightTransform.rotate(180, 0, 0, 1);

    OrbitalLight* light_2 = new OrbitalLight(GL_LIGHT1, lightTransform, 10.0f, 5.0f, 0.01f);

    //GLfloat lightSpec_2[] = {1.0f, 0.8f, 0.0f, 1.0f};
    GLfloat lightSpec_2[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat lightDif_2[] = {0.95f, 0.8f, 0.75f, 1.0f};
    //GLfloat lightDif_2[] = {0.7f, 0.7f, 0.7f, 1.0f};
    GLfloat lightAmb_2[] = {0.025f, 0.020f, 0.00f, 1.0f};

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

void Scene::clearFilters() {
    m_filters.clear();
}

void Scene::initializeGL()
{
    /*for(int i=0; i<m_lights.length(); i++) {
        m_lights[i]->initializeGL();
    }*/

    for(int i=0; i<m_objects.size(); i++) {
        m_objects[i]->initializeGL();
    }

    // Need to attach our emission image to the first frame buffer, which is not known before runtime!
    if(m_filters.length() > 0) {
        glBindFramebuffer(GL_FRAMEBUFFER, m_filters[0]->getFrameBuffer());
    }
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, ShaderManager::getInstance().getGlowTexture(), 0);
    GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0,
                            GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2,drawBuffers);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Scene::resizeGL(int width, int height) {
    for(int i=0; i<m_filters.length(); i++) {
        m_filters[i]->resizeGL(width, height);
    }
    ShaderManager::getInstance().resizeGL(width, height);
}

void Scene::paintGL()
{
    QElapsedTimer timer;
    timer.start();

    ShaderManager::getInstance().initializeGL();

    for(int render_pass=0; render_pass<m_filters.length()+1; render_pass++)
    {
        // Set correct frame buffer to render to.
        if(render_pass < m_filters.length())
        {
            glBindFramebuffer(GL_FRAMEBUFFER, m_filters[render_pass]->getFrameBuffer());
        }
        else
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        // Ensure correct read buffer is in place. This should be automatic?
        if( render_pass > 0 ) {
            glBindFramebuffer(GL_READ_FRAMEBUFFER, m_filters[render_pass-1]->getFrameBuffer());
        }

        // Clear buffers correctly. This deals with the special emissions case.
        // Couldn't get https://www.opengl.org/wiki/GLAPI/glClearBuffer to work...
        if(render_pass == 0) {
            GLenum bufferOne[] = {GL_COLOR_ATTACHMENT0};
            glDrawBuffers(1, bufferOne);
            glClearColor(0.0, 0.0, 0.0, 1.0);
            GLenum bufferTwo[] = {GL_COLOR_ATTACHMENT1};
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glDrawBuffers(1, bufferTwo);
            glClearColor(0.0, 0.0, 0.0, 1.0);
            glClear(GL_COLOR_BUFFER_BIT);
            GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0,
                                    GL_COLOR_ATTACHMENT1};
            glDrawBuffers(2,drawBuffers);
        } else {
            GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0};
            glDrawBuffers(1,drawBuffers);
            glClearColor(1.0, 1.0, 1.0, 1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

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
        }

        if(m_filters.length() == 0) {
            // OpenGL will merge all color attachments for final result. This removes the second attachment.
            GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0};
            glDrawBuffers(1,drawBuffers);
        }
    }

    //qDebug() << "Draw time: " << timer.nsecsElapsed()/1000000.0f << "µs";
}

int Scene::getLightCount()
{
    return m_lights.length();
}
