#include "shadermultipasssss.h"

ShaderMultipassSSS::ShaderMultipassSSS(QString vshader, QString fshader) :
    Shader(vshader, fshader, 3),
    m_fbos(new GLuint[3]),
    m_fbo_textures(new GLuint[3]),
    m_fbo_depths(new GLuint[3]),
    m_blur_vertices(0),
    m_attribute_v_coord_postproc(0),
    m_rim_multiplier(0.3f),
    m_uniform_fbo_width(new GLint[3]),
    m_uniform_fbo_height(new GLint[3]),
    m_uniform_fbo_texture(new GLint[3]),
    m_uniform_fbo_depth(new GLint[3]),
    m_uniform_rim_multiplier(0),
    m_result_fbo(0)
{
    init();
}

ShaderMultipassSSS::~ShaderMultipassSSS()
{
    glDeleteTextures(m_passes, m_fbo_textures);
    glDeleteTextures(m_passes, m_fbo_depths);
    glDeleteFramebuffers(m_passes, m_fbos);
    delete m_fbos;
    delete m_fbo_textures;
    delete m_fbo_depths;
    delete m_uniform_fbo_width;
    delete m_uniform_fbo_height;
    delete m_uniform_fbo_texture;
    delete m_uniform_fbo_depth;
}

void ShaderMultipassSSS::init()
{
    glDeleteProgram(m_programs[0]);
    glDeleteProgram(m_programs[1]);
    m_programs[0] = ShaderLoader::loadProgram("../../../../shaders/Basic.vsh", "../../../../shaders/SSSBackface.fsh");
    m_programs[1] = ShaderLoader::loadProgram("../../../../shaders/Basic.vsh", "../../../../shaders/SSSFrontface.fsh");
    m_blur_program = ShaderLoader::loadProgram("../../../../filters/Through.vsh", "../../../../shaders/SSSBlur.fsh");
    for(int i=0; i<m_passes; i++) {
        bindAttributes(i);
        setupFBO(i);
    }

    m_uniform_fbo_width[2] = glGetUniformLocation(m_blur_program, "fbo_width");
    if (m_uniform_fbo_width[2] == -1) {
        qWarning() << "Could not bind uniform fbo_width";
    }

    m_uniform_fbo_height[2] = glGetUniformLocation(m_blur_program, "fbo_height");
    if (m_uniform_fbo_height[2] == -1) {
        qWarning() << "Could not bind uniform fbo_height";
    }

    m_uniform_fbo_texture[2] = glGetUniformLocation(m_blur_program, "fbo_texture");
    if (m_uniform_fbo_texture[2] == -1) {
        qWarning() << "Could not bind uniform fbo_texture";
    }

    m_uniform_fbo_depth[2] = glGetUniformLocation(m_blur_program, "fbo_depth");
    if (m_uniform_fbo_depth[2] == -1) {
        qWarning() << "Could not bind uniform fbo_depth";
    }

    m_attribute_v_coord_postproc = glGetAttribLocation(m_blur_program, "v_coord");
    if (m_attribute_v_coord_postproc == -1) {
        qWarning() << "Could not bind attribute v_coord";
    }

    /* Init Drawing Mesh */
    GLfloat fbo_vertices[] = {
        -1, -1,
         1, -1,
        -1,  1,
         1,  1,
    };
    glGenBuffers(1, &m_blur_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, m_blur_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fbo_vertices), fbo_vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
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

    for(int i=0; i<m_passes; i++) {
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

    if(pass > 0) {
        m_uniform_fbo_width[pass-1] = glGetUniformLocation(m_programs[pass], "fbo_width");
        if (m_uniform_fbo_width[pass-1] == -1) {
            qWarning() << "Could not bind uniform fbo_width";
        }

        m_uniform_fbo_height[pass-1] = glGetUniformLocation(m_programs[pass], "fbo_height");
        if (m_uniform_fbo_height[pass-1] == -1) {
            qWarning() << "Could not bind uniform fbo_height";
        }

        m_uniform_fbo_texture[pass-1] = glGetUniformLocation(m_programs[pass], "fbo_texture");
        if (m_uniform_fbo_texture[pass-1] == -1) {
            qWarning() << "Could not bind uniform fbo_texture";
        }

        m_uniform_fbo_depth[pass-1] = glGetUniformLocation(m_programs[pass], "fbo_depth");
        if (m_uniform_fbo_depth[pass-1] == -1) {
            qWarning() << "Could not bind uniform fbo_depth";
        }
    }

    if(pass == 1) {
        m_uniform_rim_multiplier = glGetUniformLocation(m_programs[pass], "rim_multiplier");
        if (m_uniform_rim_multiplier == -1) {
            qWarning() << "Could not bind uniform rim_multiplier";
        }
    }
}

void ShaderMultipassSSS::updateAttributes(int pass)
{
    Shader::updateAttributes(pass);

    if(pass > 0) {
        glActiveTexture(GL_TEXTURE0 + 4);
        glBindTexture(GL_TEXTURE_2D, m_fbo_textures[(pass == 1) ? 0 : 2]);
        glActiveTexture(GL_TEXTURE0 + 5);
        glBindTexture(GL_TEXTURE_2D, m_fbo_depths[(pass == 1) ? 0 : 2]);

        glUniform1i(m_uniform_fbo_width[pass-1], m_screen_width);
        glUniform1i(m_uniform_fbo_height[pass-1], m_screen_height);
        glUniform1i(m_uniform_fbo_texture[pass-1], 4);
        glUniform1i(m_uniform_fbo_depth[pass-1], 5);
    }
    if(pass == 1) {
        glUniform1f(m_uniform_rim_multiplier, m_rim_multiplier);
    }
}

void ShaderMultipassSSS::beginGL(int pass)
{
    if(pass == 0) {
        glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &m_result_fbo);
        glCullFace(GL_FRONT);
    }
    if(pass < 2) {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbos[pass]);
        if(pass == 1) {
            glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbos[0]);
            GLfloat emission[4];
            glGetMaterialfv(GL_FRONT, GL_EMISSION, emission);
            glClearColor(emission[0], emission[1], emission[2], emission[3]);
        } else {
            glClearColor(0.0, 0.0, 0.0, 1.0);
        }

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    }
    else {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbos[2]);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_result_fbo);
    }
    Shader::beginGL(pass);
}

void ShaderMultipassSSS::endGL(int pass)
{
    glCullFace(GL_BACK);

    if(pass == 1) {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbos[2]);

        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        glUseProgram(m_blur_program);

        glActiveTexture(GL_TEXTURE0 + 6);
        glBindTexture(GL_TEXTURE_2D, m_fbo_textures[1]);
        glActiveTexture(GL_TEXTURE0 + 7);
        glBindTexture(GL_TEXTURE_2D, m_fbo_depths[1]);

        glUniform1i(m_uniform_fbo_width[2], m_screen_width);
        glUniform1i(m_uniform_fbo_height[2], m_screen_height);
        glUniform1i(m_uniform_fbo_texture[2], 6);
        glUniform1i(m_uniform_fbo_depth[2], 7);

        glEnableVertexAttribArray(m_attribute_v_coord_postproc);

        glBindBuffer(GL_ARRAY_BUFFER, m_blur_vertices);
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

    glClearColor(1.0, 1.0, 1.0, 1.0);
}
