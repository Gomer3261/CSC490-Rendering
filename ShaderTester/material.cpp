#include "material.h"

Material::Material() :
    m_count(0),
    m_name(""),
    m_ambient(),            // Ka r g b
    m_diffuse(),            // Kd r g b
    m_specular(),           // Ks r g b
    m_shininess(),          // Ns value (0 - 1000)
    m_emission(),           // Ke r g b
    m_dissolve(),           // d dissolve depth
    m_optical_density(),    // Ni optical density
    m_illumination_model(2),// illum #
    m_diffuse_tex(0),
    m_bump_tex(0),
    m_normal_tex(0),
    m_specular_tex(0)
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
    m_specular[3] = 1.0f;

    m_shininess = 0.001f;

    m_emission[0] = 0.0f;
    m_emission[1] = 0.0f;
    m_emission[2] = 0.0f;
    m_emission[3] = 1.0f;

    // illum #
    m_illumination_model = 2;
    /*
     * 0		Color on and Ambient off
     * 1		Color on and Ambient on
     * 2		Highlight on
     * 3        Emission
     * 4        Simple SSS
     * 5        Real SSS
     * 6        Highlight off Emission
     */
}

Material::~Material()
{
    if(m_diffuse_tex != 0) {
        glDeleteTextures(1, &m_diffuse_tex);
    }
    if(m_bump_tex != 0) {
        glDeleteTextures(1, &m_bump_tex);
    }
    if(m_normal_tex != 0) {
        glDeleteTextures(1, &m_normal_tex);
    }
    if(m_specular_tex != 0) {
        glDeleteTextures(1, &m_specular_tex);
    }
}

void Material::initializeGL()
{

}

void Material::beginGL()
{
    //GL_COLOR_MATERIAL is used to control color per vertex.
    //no need to use it.
    //glEnable(GL_COLOR_MATERIAL);

    /*GLfloat ambient_empty[4] = {0.1f, 0.1f, 0.1f, 1.0f};
    GLfloat diffuse_empty[4] = {0.6f, 0.6f, 0.6f, 1.0f};
    GLfloat specular_empty[4] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat emission_empty[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_empty);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_empty);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_empty);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 1);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission_empty);*/

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m_shininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, m_emission);

    if(m_diffuse_tex != 0) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_diffuse_tex);
    }
    if(m_bump_tex != 0) {
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, m_bump_tex);
    }
    if(m_normal_tex != 0) {
        glActiveTexture(GL_TEXTURE0 + 2);
        glBindTexture(GL_TEXTURE_2D, m_normal_tex);
    }
    if(m_specular_tex != 0) {
        glActiveTexture(GL_TEXTURE0 + 3);
        glBindTexture(GL_TEXTURE_2D, m_specular_tex);
    }
}

void Material::endGL()
{
    //glDisable(GL_COLOR_MATERIAL);
}

QString Material::getName()
{
    return m_name;
}

void Material::setAmbient(float r, float g, float b) {
    m_ambient[0] = r;
    m_ambient[1] = g;
    m_ambient[2] = b;
}

void Material::setDiffuse(float r, float g, float b) {
    m_diffuse[0] = r;
    m_diffuse[1] = g;
    m_diffuse[2] = b;
}

void Material::setDiffuseTexture(QImage image_file) {
    glGenTextures(1, &m_diffuse_tex);
    glBindTexture(GL_TEXTURE_2D, m_diffuse_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_file.width(), image_file.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image_file.mirrored().convertToFormat(QImage::Format_RGB888).bits());

    /*float pixels[] = {
        0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f
    };
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_FLOAT, pixels);*/
}

void Material::setSpecular(float r, float g, float b) {
    m_specular[0] = r;
    m_specular[1] = g;
    m_specular[2] = b;
}

void Material::setSpecularTexture(QImage image_file) {
    glGenTextures(1, &m_specular_tex);
    glBindTexture(GL_TEXTURE_2D, m_specular_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_file.width(), image_file.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image_file.mirrored().convertToFormat(QImage::Format_RGB888).bits());
}

void Material::setShininess(float shininess) {
    m_shininess = shininess;
}

void Material::setShininess(int shininess) {
    m_shininess = shininess/1000.0f;
}

void Material::setEmission(float r, float g, float b) {
    m_emission[0] = r;
    m_emission[1] = g;
    m_emission[2] = b;
}

void Material::setDissolve(float d) {
    m_dissolve = d;
}

void Material::setOpticalDensity(float d) {
    m_optical_density = d;
}

void Material::setAlpha(float alpha) {
    m_ambient[3] = alpha;
    m_diffuse[3] = alpha;
    m_specular[3] = alpha;
    m_emission[3] = alpha;
}

void Material::setIlluminationModel(int model) {
    m_illumination_model = model;
}

void Material::setBumpTexture(QImage image_file) {
    glGenTextures(1, &m_bump_tex);
    glBindTexture(GL_TEXTURE_2D, m_bump_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_file.width(), image_file.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image_file.mirrored().convertToFormat(QImage::Format_RGB888).bits());
}

void Material::setNormalTexture(QImage image_file) {
    glGenTextures(1, &m_normal_tex);
    glBindTexture(GL_TEXTURE_2D, m_normal_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_file.width(), image_file.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image_file.mirrored().convertToFormat(QImage::Format_RGB888).bits());
}
