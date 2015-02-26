#include "shaderloader.h"

GLuint ShaderLoader::loadShader(QString shader_file, GLuint type)
{
    GLint compile_ok = GL_FALSE;

    QFileInfo shader_file_info(shader_file);
    if(!shader_file_info.exists()) {
        qWarning() << "Vertex Shader source file " << shader_file << " does not exist";
        return 0;
    }

    QFile file(shader_file);
    GLchar* source;
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Vertex Shader source file " << shader_file << " failed to open";
        return 0;
    }

    source = new GLchar[file.size()+1];
    file.read(source, file.size());
    source[file.size()] = '\0'; // Need to append a null value to the end of the string, or GLSL will read past it.
    file.close();

    GLuint shader = glCreateShader(type);
    const GLchar* sources[3] = {
#ifdef GL_ES_VERSION_2_0
        "#version 100\n"
        "#define GLES2\n",
#else
        "#version 120\n",
#endif
        // GLES2 precision specifiers
#ifdef GL_ES_VERSION_2_0
        // Define default float precision for fragment shaders:
        (type == GL_FRAGMENT_SHADER) ?
        "#ifdef GL_FRAGMENT_PRECISION_HIGH\n"
        "precision highp float;\n"
        "#else\n"
        "precision mediump float;\n"
        "#endif\n"
        : "",
        // Note: OpenGL ES automatically defines this:
        // #define GL_ES
#else
        // Ignore GLES 2 precision specifiers:
        "#define lowp\n"
        "#define mediump\n"
        "#define highp\n",
#endif
        source
    };
    glShaderSource(shader, 3, sources, NULL);
    free(source);

    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_ok);
    if (0 == compile_ok)
    {
        qWarning() << "Error compiling shader: " << shader_file;
        ShaderLoader::print_log(shader);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint ShaderLoader::loadProgram(QString vshader, QString fshader)
{
    GLint link_ok = GL_FALSE, validate_ok = GL_FALSE;
    GLuint program, vertex_shader, fragment_shader;

    if ((vertex_shader = ShaderLoader::loadShader(vshader, GL_VERTEX_SHADER))   == 0) return 0;
    if ((fragment_shader = ShaderLoader::loadShader(fshader, GL_FRAGMENT_SHADER)) == 0) return 0;

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
    if (!link_ok) {
        qWarning() << "glLinkProgram failure: ";
        ShaderLoader::print_log(program);
        return 0;
    }
    glValidateProgram(program);
    glGetProgramiv(program, GL_VALIDATE_STATUS, &validate_ok);
    if (!validate_ok) {
        qWarning() << "glValidateProgram failure: ";
        ShaderLoader::print_log(program);
    }

    return program;
}

void ShaderLoader::print_log(GLuint object)
{
    GLint log_length = 0;
    if (glIsShader(object))
        glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
    else if (glIsProgram(object))
        glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
    else {
        qWarning() << "printlog: Not a shader or a program\n";
        return;
    }

    char* log = (char*)malloc(log_length);

    if (glIsShader(object))
        glGetShaderInfoLog(object, log_length, NULL, log);
    else if (glIsProgram(object))
        glGetProgramInfoLog(object, log_length, NULL, log);

    qWarning() << log;
    free(log);
    return;
}

