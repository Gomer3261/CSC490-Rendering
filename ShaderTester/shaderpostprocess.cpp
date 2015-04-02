#include "shaderpostprocess.h"

ShaderPostProcess::ShaderPostProcess(QString vshader, QString fshader, bool draw_depth) :
    m_passes(1),
    m_fbos(new GLuint[1]),
    m_fbo_textures(new GLuint[1]),
    m_fbo_depths(new GLuint[1]),
    m_fbo_height(1),
    m_fbo_width(1),
    m_draws_depth(draw_depth),
    m_program_postproc(new GLuint[1]),
    m_attribute_v_coord_postproc(new GLint[1]),
    m_uniform_fbo_width(new GLint[1]),
    m_uniform_fbo_height(new GLint[1]),
    m_uniform_fbo_render_texture(new GLint[1]),
    m_uniform_fbo_depth_texture(new GLint[1])
{
    init(vshader, fshader);
}

ShaderPostProcess::ShaderPostProcess(QString vshader, QString fshader, int passes, bool draw_depth) :
    m_passes(passes),
    m_fbos(new GLuint[passes]),
    m_fbo_textures(new GLuint[passes]),
    m_fbo_depths(new GLuint[passes]),
    m_fbo_height(1),
    m_fbo_width(1),
    m_draws_depth(draw_depth),
    m_program_postproc(new GLuint[passes]),
    m_attribute_v_coord_postproc(new GLint[passes]),
    m_uniform_fbo_width(new GLint[passes]),
    m_uniform_fbo_height(new GLint[passes]),
    m_uniform_fbo_render_texture(new GLint[passes]),
    m_uniform_fbo_depth_texture(new GLint[passes])
{
    init(vshader, fshader);
}

ShaderPostProcess::ShaderPostProcess(int screen_width, int screen_height, QString vshader, QString fshader, bool draw_depth) :
    m_passes(1),
    m_fbos(new GLuint[1]),
    m_fbo_textures(new GLuint[1]),
    m_fbo_depths(new GLuint[1]),
    m_fbo_height(screen_height),
    m_fbo_width(screen_width),
    m_draws_depth(draw_depth),
    m_program_postproc(new GLuint[1]),
    m_attribute_v_coord_postproc(new GLint[1]),
    m_uniform_fbo_width(new GLint[1]),
    m_uniform_fbo_height(new GLint[1]),
    m_uniform_fbo_render_texture(new GLint[1]),
    m_uniform_fbo_depth_texture(new GLint[1])
{
    init(vshader, fshader);
}

void ShaderPostProcess::init(QString vshader, QString fshader)
{
    int i;
    for(i=0; i<m_passes; i++) {
        setupFBO(i);
        setupShaders(vshader, fshader, i);
    }

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
}

void ShaderPostProcess::setupFBO(int pass)
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_fbo_width, m_fbo_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, m_fbo_width, m_fbo_height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);


    /* Create Framebuffer for two generated textures */
    glGenFramebuffers(1, &m_fbos[pass]);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbos[pass]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fbo_textures[pass], 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_fbo_depths[pass], 0);
    GLenum status;
    if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
        qDebug() << "glCheckFramebufferStatus: error " << status;
        return;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ShaderPostProcess::~ShaderPostProcess()
{
    int i;
    glDeleteTextures(m_passes, m_fbo_textures);
    glDeleteTextures(m_passes, m_fbo_depths);
    glDeleteFramebuffers(m_passes, m_fbos);
    for(i=0; i<m_passes; i++) {
        glDeleteProgram(m_program_postproc[i]);
    }
    glDeleteBuffers(1, &m_vbo_fbo_vertices);

    delete m_fbos;
    delete m_fbo_textures;
    delete m_fbo_depths;

    delete m_program_postproc;
    delete m_attribute_v_coord_postproc;
    delete m_uniform_fbo_width;
    delete m_uniform_fbo_height;
    delete m_uniform_fbo_render_texture;
    delete m_uniform_fbo_depth_texture;
}

GLuint ShaderPostProcess::setupShaders(QString vshader, QString fshader, int pass)
{
    qDebug() << "Building filter: " << fshader << " pass: " << pass;

    if(m_passes > 1) {
        QString pass_vshader = vshader.left(vshader.lastIndexOf('.'));
        pass_vshader.append(".pass");
        pass_vshader.append(QString::number(pass));
        pass_vshader.append(vshader.right(vshader.length() - vshader.lastIndexOf('.')));

        QString pass_fshader = fshader.left(fshader.lastIndexOf('.'));
        pass_fshader.append(".pass");
        pass_fshader.append(QString::number(pass));
        pass_fshader.append(fshader.right(fshader.length() - fshader.lastIndexOf('.')));

        m_program_postproc[pass] = ShaderLoader::loadProgram(pass_vshader, pass_fshader);
    }
    else {
        m_program_postproc[pass] = ShaderLoader::loadProgram(vshader, fshader);
    }

    this->bindAttributes(pass);

    return m_program_postproc[pass];
}

GLuint ShaderPostProcess::bindAttributes(int pass)
{
    m_attribute_v_coord_postproc[pass] = glGetAttribLocation(m_program_postproc[pass], "v_coord");
    if (m_attribute_v_coord_postproc[pass] == -1) {
        qWarning() << "Could not bind attribute v_coord";
    }

    m_uniform_fbo_width[pass] = glGetUniformLocation(m_program_postproc[pass], "fbo_width");
    if (m_uniform_fbo_width[pass] == -1) {
        qWarning() << "Could not bind uniform fbo_width";
    }

    m_uniform_fbo_height[pass] = glGetUniformLocation(m_program_postproc[pass], "fbo_height");
    if (m_uniform_fbo_height[pass] == -1) {
        qWarning() << "Could not bind uniform fbo_height";
    }

    m_uniform_fbo_render_texture[pass] = glGetUniformLocation(m_program_postproc[pass], "fbo_render");
    if (m_uniform_fbo_render_texture[pass] == -1) {
        qWarning() << "Could not bind uniform fbo_render";
    }

    m_uniform_fbo_depth_texture[pass] = glGetUniformLocation(m_program_postproc[pass], "fbo_depth");
    if (m_uniform_fbo_depth_texture[pass] == -1) {
        qWarning() << "Could not bind uniform fbo_depth";
    }

    return 1;
}

void ShaderPostProcess::resizeGL(int screen_width, int screen_height)
{
    m_fbo_width = screen_width;
    m_fbo_height = screen_height;

    for(int i=0; i<m_passes; i++) {
        glBindTexture(GL_TEXTURE_2D, m_fbo_textures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screen_width, screen_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glBindTexture(GL_TEXTURE_2D, 0);

        glBindTexture(GL_TEXTURE_2D, m_fbo_depths[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, screen_width, screen_height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

GLuint ShaderPostProcess::getFrameBuffer()
{
    return m_fbos[0];
}

void ShaderPostProcess::paintGL()
{
    GLint result_fbo;
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &result_fbo);
    for(int i=1; i<=m_passes; i++) {
        if(i == m_passes) {
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, result_fbo);
        } else {
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbos[i]);
        }

        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        glUseProgram(m_program_postproc[i-1]);

        this->updateAttributes(i-1);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo_fbo_vertices);
        glVertexAttribPointer(
            m_attribute_v_coord_postproc[i-1],  // attribute
            2,                  // number of elements per vertex, here (x,y)
            GL_FLOAT,           // the type of each element
            GL_FALSE,           // take our values as-is
            0,                  // no extra data between each position
            0                   // offset of first element
        );
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDisableVertexAttribArray(m_attribute_v_coord_postproc[i-1]);
    }

    if(!m_draws_depth) {
        glBlitFramebuffer(0, 0, m_fbo_width, m_fbo_height, 0, 0, m_fbo_width, m_fbo_height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    }
}

void ShaderPostProcess::updateAttributes(int pass)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_fbo_textures[pass]);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, m_fbo_depths[pass]);

    glUniform1i(m_uniform_fbo_width[pass], m_fbo_width);
    glUniform1i(m_uniform_fbo_height[pass], m_fbo_height);
    glUniform1i(m_uniform_fbo_render_texture[pass], 0);
    glUniform1i(m_uniform_fbo_depth_texture[pass], 1);
    glEnableVertexAttribArray(m_attribute_v_coord_postproc[pass]);
}



