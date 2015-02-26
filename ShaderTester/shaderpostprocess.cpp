#include "shaderpostprocess.h"

ShaderPostProcess::ShaderPostProcess(int screen_width, int screen_height, QString vshader, QString fshader)
{
    /* init_buffers */

    /* Create color texture pointer */
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &m_fbo_texture);
    glBindTexture(GL_TEXTURE_2D, m_fbo_texture);

    /* Initialize color texture settings */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    /* Allocate color texture memory */
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screen_width, screen_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);


    /* Create depth texture pointer */
    glGenTextures(1, &m_fbo_depth);
    glBindTexture(GL_TEXTURE_2D, m_fbo_depth);

    /* Initialize depth texture settings */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

    /* Allocate depth texture memory */
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, screen_width, screen_height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);


    /* Create Framebuffer for two generated textures */
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fbo_texture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_fbo_depth, 0);
    GLenum status;
    if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
        qDebug() << "glCheckFramebufferStatus: error " << status;
        return;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    /* Init Drawing Mesh */
    GLfloat fbo_vertices[] = {
        -1, -1,
         1, -1,
        -1,  1,
         1,  1,
    };
    glGenBuffers(1, &m_vbo_fbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_fbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fbo_vertices), fbo_vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    /* Init Shader */
    setupShader(vshader, fshader);
}

ShaderPostProcess::~ShaderPostProcess()
{
    glDeleteTextures(1, &m_fbo_texture);
    glDeleteTextures(1, &m_fbo_depth);
    glDeleteFramebuffers(1, &m_fbo);
    glDeleteBuffers(1, &m_vbo_fbo_vertices);
    glDeleteProgram(m_program_postproc);
}

GLuint ShaderPostProcess::setupShader(QString vshader, QString fshader)
{
    qDebug() << "Building filter: " << fshader;
    m_program_postproc = ShaderLoader::loadProgram(vshader, fshader);

    this->bindAttributes();

    return m_program_postproc;
}

GLuint ShaderPostProcess::bindAttributes()
{
    m_attribute_v_coord_postproc = glGetAttribLocation(m_program_postproc, "v_coord");
    if (m_attribute_v_coord_postproc == -1) {
        qWarning() << "Could not bind attribute v_coord";
    }

    m_uniform_fbo_width = glGetUniformLocation(m_program_postproc, "fbo_width");
    if (m_uniform_fbo_width == -1) {
        qWarning() << "Could not bind uniform fbo_width";
    }

    m_uniform_fbo_height = glGetUniformLocation(m_program_postproc, "fbo_height");
    if (m_uniform_fbo_height == -1) {
        qWarning() << "Could not bind uniform fbo_height";
    }

    m_uniform_fbo_render_texture = glGetUniformLocation(m_program_postproc, "fbo_render");
    if (m_uniform_fbo_render_texture == -1) {
        qWarning() << "Could not bind uniform fbo_render";
    }

    m_uniform_fbo_depth_texture = glGetUniformLocation(m_program_postproc, "fbo_depth");
    if (m_uniform_fbo_depth_texture == -1) {
        qWarning() << "Could not bind uniform fbo_depth";
    }

    return 1;
}

void ShaderPostProcess::resizeGL(int screen_width, int screen_height)
{
    m_fbo_width = screen_width;
    m_fbo_height = screen_height;

    glBindTexture(GL_TEXTURE_2D, m_fbo_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screen_width, screen_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindTexture(GL_TEXTURE_2D, m_fbo_depth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, screen_width, screen_height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint ShaderPostProcess::getFrameBuffer()
{
    return m_fbo;
}

void ShaderPostProcess::paintGL()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_program_postproc);

    this->updateAttributes();

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_fbo_vertices);
    glVertexAttribPointer(
        m_attribute_v_coord_postproc,  // attribute
        2,                  // number of elements per vertex, here (x,y)
        GL_FLOAT,           // the type of each element
        GL_FALSE,           // take our values as-is
        0,                  // no extra data between each position
        0                   // offset of first element
    );
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDisableVertexAttribArray(m_attribute_v_coord_postproc);
}

void ShaderPostProcess::updateAttributes()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_fbo_texture);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, m_fbo_depth);

    glUniform1i(m_uniform_fbo_width, m_fbo_width);
    glUniform1i(m_uniform_fbo_height, m_fbo_height);
    glUniform1i(m_uniform_fbo_render_texture, 0);
    glUniform1i(m_uniform_fbo_depth_texture, 1);
    glEnableVertexAttribArray(m_attribute_v_coord_postproc);
}



