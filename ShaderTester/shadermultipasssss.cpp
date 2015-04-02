#include "shadermultipasssss.h"

ShaderMultipassSSS::ShaderMultipassSSS(QString vshader, QString fshader, QString vshader2, QString fshader2) :
    Shader(vshader, fshader, 2),
    m_fbos(new GLuint[1]),
    m_fbo_textures(new GLuint[1]),
    m_fbo_depths(new GLuint[1]),
    m_rim_multiplier(0.3f),
    m_uniform_backface_texture(0),
    m_uniform_backface_depth(0),
    m_uniform_exctinction_coefficient(new GLint[2]),
    m_uniform_rim_multiplier(new GLint[2])
{
    init(vshader2, fshader2);
}

ShaderMultipassSSS::~ShaderMultipassSSS()
{
    glDeleteTextures(m_passes-1, m_fbo_textures);
    glDeleteTextures(m_passes-1, m_fbo_depths);
    glDeleteFramebuffers(m_passes-1, m_fbos);
    delete m_fbos;
    delete m_fbo_textures;
    delete m_fbo_depths;
    delete m_uniform_exctinction_coefficient;
    delete m_uniform_rim_multiplier;
}

void ShaderMultipassSSS::init(QString vshader, QString fshader)
{
    glDeleteProgram(m_programs[1]);
    m_programs[1] = ShaderLoader::loadProgram(vshader, fshader);
    for(int i=0; i<m_passes; i++) {
        bindAttributes(i);
        if(i < m_passes-1) {
            setupFBO(i);
        }
    }
}

void ShaderMultipassSSS::setupFBO(int pass)
{
    /* Create color texture pointer */
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &m_fbo_textures[pass]);
    glBindTexture(GL_TEXTURE_2D, m_fbo_textures[pass]);

    /* Initialize color texture settings */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    /* Allocate color texture memory */
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_screen_width, m_screen_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);


    /* Create depth texture pointer */
    glGenTextures(1, &m_fbo_depths[pass]);
    glBindTexture(GL_TEXTURE_2D, m_fbo_depths[pass]);

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
    glGenFramebuffers(1, &m_fbos[pass]);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbos[pass]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fbo_textures[pass], 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_fbo_depths[pass], 0);
    GLenum status;
    if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
        qDebug() << "glCheckFramebufferStatus: error " << status << " " << pass;
        return;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShaderMultipassSSS::resizeGL(int screen_width, int screen_height) {
    Shader::resizeGL(screen_width, screen_height);

    for(int i=0; i<m_passes-1; i++) {
        glBindTexture(GL_TEXTURE_2D, m_fbo_textures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screen_width, screen_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindTexture(GL_TEXTURE_2D, m_fbo_depths[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, screen_width, screen_height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

void ShaderMultipassSSS::bindAttributes(int pass)
{
    Shader::bindAttributes(pass);

    if(pass == m_passes-1) {
        m_uniform_backface_width = glGetUniformLocation(m_programs[pass], "backface_width");
        if (m_uniform_backface_width == -1) {
            qWarning() << "Could not bind uniform backface_width";
        }

        m_uniform_backface_height = glGetUniformLocation(m_programs[pass], "backface_height");
        if (m_uniform_backface_height == -1) {
            qWarning() << "Could not bind uniform backface_height";
        }

        m_uniform_backface_texture = glGetUniformLocation(m_programs[pass], "backface_texture");
        if (m_uniform_backface_texture == -1) {
            qWarning() << "Could not bind uniform backface_texture";
        }

        m_uniform_backface_depth = glGetUniformLocation(m_programs[pass], "backface_depth");
        if (m_uniform_backface_depth == -1) {
            qWarning() << "Could not bind uniform backface_depth";
        }
    }

    m_uniform_exctinction_coefficient[pass] = glGetUniformLocation(m_programs[pass], "exctinction_coefficient");
    if (m_uniform_exctinction_coefficient[pass] == -1) {
        qWarning() << "Could not bind uniform exctinction_coefficient";
    }

    m_uniform_rim_multiplier[pass] = glGetUniformLocation(m_programs[pass], "rim_multiplier");
    if (m_uniform_rim_multiplier[pass] == -1) {
        qWarning() << "Could not bind uniform rim_multiplier";
    }
}

void ShaderMultipassSSS::updateAttributes(int pass)
{
    Shader::updateAttributes(pass);

    if(pass == m_passes-1) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_fbo_textures[pass-1]);
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, m_fbo_depths[pass-1]);

        glUniform1i(m_uniform_backface_width, m_screen_width);
        glUniform1i(m_uniform_backface_height, m_screen_height);
        glUniform1i(m_uniform_backface_texture, 0);
        glUniform1i(m_uniform_backface_depth, 1);
    }

    glUniform3f(m_uniform_exctinction_coefficient[pass], 0.6f, 0.4f, 0.8f); // Would love to get from material :D
    glUniform1f(m_uniform_rim_multiplier[pass], m_rim_multiplier);
}

void ShaderMultipassSSS::beginGL(int pass)
{
    if(pass == 0) {
        glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &m_result_fbo);
    }
    if(pass < m_passes-1) {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbos[pass]);
        glCullFace(GL_FRONT);

        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    } else {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbos[pass-1]);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_result_fbo);
    }
    Shader::beginGL(pass);
}

void ShaderMultipassSSS::endGL(int pass)
{
    (void)pass;
    glCullFace(GL_BACK);
}
