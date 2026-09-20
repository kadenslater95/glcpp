// Copyright 2026 Kaden Slater

#include <GL/glew.h>
#include <GL/glut.h>

#include <Eigen/Dense>

#define _USE_MATH_DEFINES
#include <cmath>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


GLuint shaderProgram, VAO;
GLuint VBOs[2];
GLint modelLoc, normalMatrixLoc, cameraLoc, projectionLoc;
GLint lightPositionLoc, cameraPositionLoc, lightColorLoc, objectColorLoc;

GLuint compileShader(const std::string, GLenum);
void initGeometry();
void init();

void displayFunc();


int main(int argc, char** argv) {
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 150);
    glutCreateWindow("glcpp");

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW!" << "\n";
        return -1;
    }

    init();

    glutDisplayFunc(displayFunc);

    glutMainLoop();

    return 0;
}


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

    return shader;
}


void initGeometry() {
    float positions[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    float normals[] = {
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f
    };

    float fov = M_PI / 4.0f;
    float aspect = 1.0f;
    float near = 0.1f;
    float far = 100.0;

    float cameraPosition[] = {
        0.0f, 0.0f, -15.0f
    };

    float lightPosition[] = {
        -15.0f, 10.0f, -15.0f
    };

    float lightColor[] = {
        0.8f, 0.8f, 0.8f
    };

    float objectColor[] = {
        0.5f, 0.5f, 0.9f
    };

    Eigen::Matrix4f modelMtx = Eigen::Matrix4f::Identity();
    Eigen::Matrix4f normalMtx = modelMtx.inverse().transpose();

    Eigen::Matrix4f cameraMtx = Eigen::Matrix4f::Identity();

    cameraMtx(0, 0) = cameraPosition[0];
    cameraMtx(1, 0) = cameraPosition[1];
    cameraMtx(2, 0) = cameraPosition[2];

    // Camera Up is <0, 1, 0>
    cameraMtx(2, 2) = 0.0f;
    cameraMtx(2, 1) = 1.0f;

    Eigen::Matrix4f projectionMtx = Eigen::Matrix4f::Zero();

    projectionMtx(1, 1) = 1.0f / std::tan(fov / 2.0f);
    projectionMtx(0, 0) = projectionMtx(1, 1) / aspect;
    projectionMtx(2, 2) = -(far + near) / (far - near);

    projectionMtx(3, 3) = 0.0f;
    projectionMtx(3, 2) = -1.0f;
    projectionMtx(2, 3) = -2.0f * far * near / (far - near);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, VBOs);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3*sizeof(float),
        (const void *) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        3*sizeof(float),
        (const void *) 0);
    glEnableVertexAttribArray(1);

    // Unselect any vertex buffer object or attribute pointer array
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    modelLoc = glGetUniformLocation(shaderProgram, "uModel");
    cameraLoc = glGetUniformLocation(shaderProgram, "uCamera");
    projectionLoc = glGetUniformLocation(shaderProgram, "uProjection");

    normalMatrixLoc = glGetUniformLocation(shaderProgram, "uNormalMatrix");

    glUniformMatrix4fv(
        modelLoc,
        1,
        GL_FALSE,
        modelMtx.data());
    glUniformMatrix4fv(
        modelLoc,
        1,
        GL_FALSE,
        cameraMtx.data());
    glUniformMatrix4fv(
        modelLoc,
        1,
        GL_FALSE,
        projectionMtx.data());

    glUniformMatrix4fv(
        modelLoc,
        1,
        GL_FALSE,
        normalMtx.data());

    cameraPositionLoc = glGetUniformLocation(shaderProgram, "uCameraPosition");
    lightPositionLoc = glGetUniformLocation(shaderProgram, "uLightPosition");

    lightColorLoc = glGetUniformLocation(shaderProgram, "uLightColor");
    objectColorLoc = glGetUniformLocation(shaderProgram, "uObjectColor");

    glUniform3fv(
        cameraPositionLoc,
        1,
        cameraPosition);
    glUniform3fv(
        lightPositionLoc,
        1,
        lightPosition);

    glUniform3fv(
        lightColorLoc,
        1,
        lightColor);
    glUniform3fv(
        objectColorLoc,
        1,
        objectColor);
}


void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);

    GLuint vertexShader = compileShader(
        "shaders/perspective.vert",
        GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(
        "shaders/blinn_phong.frag",
        GL_FRAGMENT_SHADER);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    initGeometry();
}


void displayFunc() {
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glutSwapBuffers();
}
