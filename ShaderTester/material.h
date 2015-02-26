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

private:
    Material();


    unsigned int m_count;

    GLfloat m_ambient[4];
    GLfloat m_diffuse[4];
    GLfloat m_specular[4];
    GLfloat m_shininess;
    GLfloat m_emission[4];
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
