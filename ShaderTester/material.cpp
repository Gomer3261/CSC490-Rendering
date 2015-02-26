#include "material.h"

Material::Material() :
    m_count(0),
    m_ambient(),
    m_diffuse(),
    m_specular(),
    m_shininess(0.001f),
    m_emission()
{
    m_ambient[0] = 1.0f;
    m_ambient[1] = 1.0f;
    m_ambient[2] = 1.0f;
    m_ambient[3] = 1.0f;

    m_diffuse[0] = 1.0f;
    m_diffuse[1] = 1.0f;
    m_diffuse[2] = 1.0f;
    m_diffuse[3] = 1.0f;

    m_specular[0] = 0.0f;
    m_specular[1] = 0.0f;
    m_specular[2] = 0.0f;
    m_specular[3] = 0.0f;

    m_emission[0] = 0.0f;
    m_emission[1] = 0.0f;
    m_emission[2] = 0.0f;
    m_emission[3] = 0.0f;
}

Material::~Material()
{
}

void Material::initializeGL()
{

}

void Material::beginGL()
{
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m_shininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, m_emission);

    glEnable(GL_COLOR_MATERIAL);
}

void Material::endGL()
{
    glDisable(GL_COLOR_MATERIAL);
}
