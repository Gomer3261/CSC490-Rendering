#ifndef MATERIAL_H
#define MATERIAL_H

#include "qgl.h"
#include <QDebug>

class Material;

class Material
{
public:
    class Ptr;
    static Ptr create();
    ~Material();

    void initializeGL();
    void beginGL();
    void endGL();

    QString getName();

    void setAmbient(float r, float g, float b);     // Ka r g b
    void setDiffuse(float r, float g, float b);     // Kd r g b
    void setSpecular(float r, float g, float b);    // Ks r g b
    void setShininess(float shininess);             // Ns value (0 - 1)
    void setShininess(int shininess);               // Ns value (0 - 1000)
    void setEmission(float r, float g, float b);    // Ke r g b
    void setDissolve(float d);                      // d disolve depth (0 - 1)
    void setOpticalDensity(float d);                // Ni optical density (0 - 10)
    void setIlluminationModel(int model);           // illum (0 - 10)
    int getIlluminationModel() { return m_illumination_model; }

private:
    Material();

    unsigned int m_count;

    QString m_name;
    GLfloat m_ambient[4];
    GLfloat m_diffuse[4];
    GLfloat m_specular[4];
    GLfloat m_shininess;
    GLfloat m_emission[4];
    GLfloat m_dissolve;
    GLfloat m_optical_density;
    int m_illumination_model;
};

class Material::Ptr {
public:
    Material* operator-> () { return p_; }
    Material& operator* () { return *p_; }
    Ptr(Material* p) : p_(p) { ++p_->m_count; }
    ~Ptr() { if(--p_->m_count == 0) delete p_; }
    Ptr(Ptr const& p) : p_(p.p_) { ++p_->m_count; }
    Ptr& operator= (Ptr const& p) {
        Material* const old = p_;
        p_ = p.p_;
        ++p_->m_count;
        if (--old->m_count == 0) delete old;
        return *this;
    }
private:
    Material* p_;
};



inline Material::Ptr Material::create() { return new Material(); }

#endif // MATERIAL_H
