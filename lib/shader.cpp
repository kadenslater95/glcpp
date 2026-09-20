// Copyright 2026 Kaden Slater

#include "glcpp/shader.h"



GLuint compileShader(std::string shaderPath, GLenum shaderType) {
    std::ifstream shader_fstream(shaderPath);

    if (!shader_fstream.is_open()) {
        std::cerr << "Failed to open shader source file!" << "\n";
        return 0;  // Note: glCreateShader returns 0 for error
    }

    std::stringstream buffer;
    buffer << shader_fstream.rdbuf();

    std::string shaderSource = buffer.str();
    const char *shaderSourcePtr = shaderSource.c_str();

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSourcePtr, NULL);

    glCompileShader(shader);

    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

    if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

        std::cout << "GLSL Shader Info Log: " << &errorLog[0] << "\n";
    }

    return shader;
}

