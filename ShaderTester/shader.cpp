#include "shader.h"

// Note to future Geoff, change your rendering system for this. Don't hack it into every shader. ><
// Derp.
GLuint Shader::m_emission_texture=0;
GLuint Shader::m_emission_depth=0;
GLuint Shader::m_emission_fbo=0;

Shader::Shader(QString vshader, QString fshader) :
    m_default_fbo(0),
    m_lighting_flags(USE_BASIC_LIGHTING),
    m_passes(2),
    m_screen_width(256),
    m_screen_height(256),
    m_programs(new GLuint[2]),
    m_uniform_lcount(new GLint[2]),
    m_uniform_ambient(new GLint[2]),
    m_uniform_diffuse(new GLint[2]),
    m_uniform_specular(new GLint[2]),
    m_uniform_emission(new GLint[2])
{
    init(vshader, fshader);
}

Shader::Shader(QString vshader, QString fshader, int passes) :
    m_default_fbo(0),
    m_lighting_flags(USE_BASIC_LIGHTING),
    m_passes(passes),
    m_programs(new GLuint[passes]),
    m_uniform_lcount(new GLint[passes]),
    m_uniform_ambient(new GLint[passes]),
    m_uniform_diffuse(new GLint[passes]),
    m_uniform_specular(new GLint[passes]),
    m_uniform_emission(new GLint[passes])
{
    init(vshader, fshader);
}

Shader::~Shader()
{
    for(int i=0; i<m_passes; i++) {
        glDeleteProgram(m_programs[i]);
    }
    if(m_emission_fbo == 0) {
        glDeleteTextures(1, &m_emission_texture);
        glDeleteTextures(1, &m_emission_depth);
        glDeleteFramebuffers(1, &m_emission_fbo);
        m_emission_fbo = 0;
    }
    delete m_programs;
    delete m_uniform_lcount;
    delete m_uniform_ambient;
    delete m_uniform_diffuse;
    delete m_uniform_specular;
    delete m_uniform_emission;
}

GLuint Shader::getEmissionTexture() {
    return m_emission_texture;
}

GLuint Shader::getEmissionDepth() {
    return m_emission_depth;
}

void Shader::init(QString vshader, QString fshader)
{
    for(int i=0; i<m_passes; i++) {
        if(i != 1) {
            m_programs[i] = ShaderLoader::loadProgram(vshader, fshader);
        } else {
            // Unfortunately, this shader needs to be used for everything.
            // I'm doing it this way, rather than re-write my rendering loop to have an extra input pass.
            m_programs[i] = ShaderLoader::loadProgram("../../../../shaders/Basic.vsh", "../../../../shaders/Glow.fsh");
        }
        bindAttributes(i);
    }

    if(m_emission_fbo == 0) {
        glEnable(GL_TEXTURE_2D);
        glGenTextures(1, &m_emission_texture);
        glBindTexture(GL_TEXTURE_2D, m_emission_texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_screen_width, m_screen_height, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, NULL);
        glBindTexture(GL_TEXTURE_2D, 0);

        /* Create depth texture pointer */
        glGenTextures(1, &m_emission_depth);
        glBindTexture(GL_TEXTURE_2D, m_emission_depth);

        /* Initialize depth texture settings */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

        /* Allocate depth texture memory */
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, m_screen_width, m_screen_height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
        glBindTexture(GL_TEXTURE_2D, 0);

        /* Create Framebuffer for two generated textures */
        glGenFramebuffers(1, &m_emission_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, m_emission_fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_emission_texture, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_emission_depth, 0);
        GLenum status;
        if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
            qDebug() << "glCheckFramebufferStatus: error " << status;
            return;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

void Shader::initializeGL() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_emission_fbo);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void Shader::resizeGL(int screen_width, int screen_height) {
    m_screen_width = screen_width;
    m_screen_height = screen_height;

    glBindTexture(GL_TEXTURE_2D, m_emission_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_screen_width, m_screen_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindTexture(GL_TEXTURE_2D, m_emission_depth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, m_screen_width, m_screen_height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Shader::beginGL(int pass)
{
    if(pass == 0) {
        glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &m_default_fbo);
    } else if(pass == 1) {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_emission_fbo);
    }
    glUseProgram(m_programs[pass]);
    updateAttributes(pass);
}

void Shader::endGL() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_default_fbo);
}

void Shader::bindAttributes(int pass)
{
    m_uniform_lcount[pass] = glGetUniformLocation(m_programs[pass], "lCount");
    if (m_uniform_lcount[pass] == -1) {
        qWarning() << "Could not bind uniform lCount";
    }

    m_uniform_ambient[pass] = glGetUniformLocation(m_programs[pass], "ambient_on");
    if (m_uniform_ambient[pass] == -1) {
        qWarning() << "Could not bind uniform ambient_on";
    }

    m_uniform_diffuse[pass] = glGetUniformLocation(m_programs[pass], "diffuse_on");
    if (m_uniform_diffuse[pass] == -1) {
        qWarning() << "Could not bind uniform diffuse_on";
    }

    m_uniform_specular[pass] = glGetUniformLocation(m_programs[pass], "specular_on");
    if (m_uniform_specular[pass] == -1) {
        qWarning() << "Could not bind uniform specular_on";
    }

    m_uniform_emission[pass] = glGetUniformLocation(m_programs[pass], "emission_on");
    if (m_uniform_emission[pass] == -1) {
        qWarning() << "Could not bind uniform emission_on";
    }
}

void Shader::updateAttributes(int pass)
{
    glUniform1i(m_uniform_lcount[pass], 2);
    glUniform1i(m_uniform_ambient[pass], (m_lighting_flags & USE_AMBIENT) > 0);
    glUniform1i(m_uniform_diffuse[pass], (m_lighting_flags & USE_DIFFUSE) > 0);
    glUniform1i(m_uniform_specular[pass], (m_lighting_flags & USE_SPECULAR) > 0);
    glUniform1i(m_uniform_emission[pass], (m_lighting_flags & USE_EMISSION) > 0);
}

