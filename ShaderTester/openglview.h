#ifndef OPENGLVIEW_H
#define OPENGLVIEW_H

#include "scene.h"

#include <QGLWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QMatrix4x4>
#include <QDebug>

class OpenGLView : public QGLWidget
{
    Q_OBJECT
public:
    explicit OpenGLView(QWidget *parent = 0);
    ~OpenGLView();

    void setScene(Scene* scene);
signals:

public slots:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void updateCameraAngle();
protected:
private:
    Scene* m_scene;

    float   m_screenWidth;
    float   m_screenHeight;
    float   m_aspectRatio;
    float   m_nearPlane;
    float   m_farPlane;
    float   m_fieldOfView;

    QMatrix4x4  m_projectionMatrix;
    QMatrix4x4  m_viewMatrix;
    QMatrix4x4  m_invViewMatrix;
    QVector3D   m_cameraPosition;

    float       m_rotationX;
    float       m_rotationY;
    float       m_distance;
    int         m_screenPreviousX;
    int         m_screenPreviousY;
};

#endif // OPENGLVIEW_H
