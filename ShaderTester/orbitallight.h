#ifndef ORIBITALLIGHTSOURCE_H
#define ORIBITALLIGHTSOURCE_H

#include "qgl.h"
#include <QDebug>
#include <QVector3D>
#include <QMatrix4x4>
#include <QtMath>

#include "light.h"

class OrbitalLight : public Light
{
public:
    OrbitalLight(int lightId, QMatrix4x4 transform, float radius, float z, float rotationRate);
    ~OrbitalLight();
    void initializeGL(GLfloat lightSpec[], GLfloat lightDif[], GLfloat lightAmb[]);
    void paintGL();
    int getLightId();
protected:
private:
    int m_lightId;
    QVector3D m_center;
    QMatrix4x4 m_transform;
    float m_radius;
    float m_angle;
    float m_rotationRate;
};

#endif // ORIBITALLIGHTSOURCE_H
