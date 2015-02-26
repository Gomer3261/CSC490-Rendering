#include "openglview.h"

OpenGLView::OpenGLView(QWidget *parent) :
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
    m_scene(NULL),
    m_projectionMatrix(),
    m_viewMatrix(),
    m_cameraPosition(),
    m_rotationX(20),
    m_rotationY(0),
    m_distance(4.0f)
{
    m_screenWidth = 0.0f;
    m_screenHeight = 0.0f;
    m_aspectRatio = 1.0f;
    m_fieldOfView = 60.0f;
    m_nearPlane = 0.1f;
    m_farPlane = 25.0f;

    updateCameraAngle();
}

OpenGLView::~OpenGLView()
{

}

void OpenGLView::setScene(Scene* scene) {
    m_scene = scene;
    m_scene->resizeGL(m_screenWidth, m_screenHeight);
}

void OpenGLView::updateCameraAngle() {
    QMatrix4x4 rotx;
    rotx.rotate(m_rotationX, 0.0f, 1.0f, 0.0f);
    QMatrix4x4 roty;
    roty.rotate(m_rotationY, 1.0f, 0.0f, 0.0f);
    m_cameraPosition = rotx * roty * QVector3D(0.0f, 0.0f, -m_distance);
}

void OpenGLView::initializeGL()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void OpenGLView::paintGL()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glMatrixMode(GL_MODELVIEW);
    m_viewMatrix.setToIdentity();
    m_viewMatrix.lookAt(m_cameraPosition,
                      QVector3D(0.0f, 0.0f, 0.0f),
                      QVector3D(0.0f, 1.0f, 0.0f));
    glLoadMatrixf(m_viewMatrix.constData());

    if(m_scene != NULL) {
        m_scene->paintGL();
    } else {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}

void OpenGLView::resizeGL(int width, int height)
{
    m_screenWidth = width;
    m_screenHeight = height;
    m_aspectRatio = m_screenWidth/m_screenHeight;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    m_projectionMatrix.setToIdentity();
    m_projectionMatrix.perspective(m_fieldOfView, m_aspectRatio, m_nearPlane, m_farPlane);
    glLoadMatrixf(m_projectionMatrix.constData());

    glMatrixMode(GL_MODELVIEW);

    if(m_scene != NULL) {
        m_scene->resizeGL(width, height);
    }
}

void OpenGLView::mousePressEvent(QMouseEvent *event)
{
    m_screenPreviousX = event->pos().x();
    m_screenPreviousY = event->pos().y();

    event->accept();
    QGLWidget::mousePressEvent(event);
}

void OpenGLView::mouseMoveEvent(QMouseEvent *event)
{
    m_rotationX += (m_screenPreviousX - event->pos().x())/1.0f;
    m_rotationY -= (m_screenPreviousY - event->pos().y())/1.0f;

    if(m_rotationX > 360.0f) m_rotationX -= 360.0f;
    if(m_rotationX < 0.0f) m_rotationX += 360.0f;
    if(m_rotationY > 70.0f) m_rotationY = 70.0f;
    if(m_rotationY < -70.0f) m_rotationY = -70.0f;

    m_screenPreviousX = event->pos().x();
    m_screenPreviousY = event->pos().y();

    updateCameraAngle();

    event->accept();
    QGLWidget::mouseMoveEvent(event);
}

void OpenGLView::mouseReleaseEvent(QMouseEvent *event)
{
    event->accept();
    QGLWidget::mouseReleaseEvent(event);
}

void OpenGLView::wheelEvent(QWheelEvent *event)
{
    m_distance = m_distance + event->delta()*0.02f;
    if(m_distance < 2.0f) m_distance = 2.0f;
    if(m_distance > 10.0f) m_distance = 10.0f;
    updateCameraAngle();
}
