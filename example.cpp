// Copyright 2026 Kaden Slater

#include <GL/glew.h>
#include <GL/glut.h>

#include <Eigen/Dense>

#define _USE_MATH_DEFINES
#include <cmath>

#include <iostream>

#include "glcpp/shader.h"


GLuint shaderProgram, VAO;
GLuint VBOs[2];
GLint modelMtxLoc, normalMtxLoc, cameraMtxLoc, projectionMtxLoc;
GLint lightPosLoc, cameraPosLoc, lightColorLoc, objectColorLoc;

void initGeometry();
void init();

void displayFunc();
void tick(int);


class Camera {
    Eigen::Matrix4f _mtx;

 public:
    Camera() {
        this->_mtx = Eigen::Matrix4f::Identity();
    }

    void getPosition(float position[3]) {
        position[0] = this->_mtx(3, 0);
        position[1] = this->_mtx(3, 1);
        position[2] = this->_mtx(3, 2);
    }

    void setPosition(float x, float y, float z) {
        this->_mtx(3, 0) = x;
        this->_mtx(3, 1) = y;
        this->_mtx(3, 2) = z;
    }

    void setDirection(float x, float y, float z) {
        Eigen::Quaternionf q(1.0f, x, y, z);
        q = q.normalized();

        Eigen::Matrix3f rot = q.toRotationMatrix();

        this->_mtx.topLeftCorner<3, 3>() = rot;
    }
};


class Light {
    float _pos[3];
    float _color[3];

 public:
    Light() {
        this->_pos[0] = 0.0f;
        this->_pos[1] = 1.0f;
        this->_pos[2] = 0.0f;

        this->_color[0] = 1.0f;
        this->_color[1] = 1.0f;
        this->_color[2] = 1.0f;
    }
};

class Scene {
    Camera* _camera;
    Light* _light;

    GLuint _shaderProgram;

    GLint _cameraMtxLoc;
    GLint _lightPosLoc;
    GLint _lightColorLoc;

 public:
    Scene(GLuint shaderProgram, Camera* camera, Light* light) {
        this->_camera = camera;
        this->_light = light;

        this->_shaderProgram = shaderProgram;

        this->_cameraMtxLoc = glGetUniformLocation(
            this->_shaderProgram,
            "uCameraMtx");

        this->_lightPosLoc = glGetUniformLocation(
            this->_shaderProgram,
            "uLightPos");

        this->_lightColorLoc = glGetUniformLocation(
            this->_shaderProgram,
            "uLightColor");
    }
};


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

    glutTimerFunc(16, tick, 0);

    glutMainLoop();

    return 0;
}


void initGeometry() {
    float positions[] = {
        1.0f, 1.0f, -5.0f,
        0.0f, -0.5f, -5.0f,
        -1.0f, 1.0f, -5.0f
    };

    float normals[] = {
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(2, VBOs);

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
}


void initUniforms(float t) {
    float fov = M_PI / 4.0f;
    float aspect = 1.0f;
    float near = 0.1f;
    float far = 100.0;

    float cameraPosition[] = {
        0.0f, 0.0f, 0.0f
    };

    float lightPosition[] = {
        0.0f, 1.0f, 0.0f
    };

    float lightColor[] = {
        0.8f, 0.8f, 0.8f
    };

    float objectColor[] = {
        0.5f, 0.5f, 0.9f
    };

    Eigen::Matrix4f modelMtx = Eigen::Matrix4f::Identity();

    // modelMtx.block<3, 3>(0, 0) = Eigen::AngleAxisf(
    //     t,
    //     Eigen::Vector3f::UnitZ()).toRotationMatrix();

    Eigen::Matrix4f normalMtx = modelMtx.inverse().transpose();

    Eigen::Matrix4f cameraMtx = Eigen::Matrix4f::Identity();

    // cameraMtx.block<3, 3>(0, 0) = Eigen::AngleAxisf(
    //     t,
    //     Eigen::Vector3f::UnitY()).toRotationMatrix();

    // cameraMtx(2, 3) = 3.0f + 3.0f*sin(t);

    // cameraMtx(0, 2) = sin(0.1*t);

    Eigen::Matrix4f projectionMtx = Eigen::Matrix4f::Zero();

    projectionMtx(1, 1) = 1.0f / std::tan(fov / 2.0f);
    projectionMtx(0, 0) = projectionMtx(1, 1) / aspect;
    projectionMtx(2, 2) = -(far + near) / (far - near);

    projectionMtx(3, 3) = 0.0f;
    projectionMtx(3, 2) = -1.0f;
    projectionMtx(2, 3) = -2.0f * far * near / (far - near);

    modelMtxLoc = glGetUniformLocation(shaderProgram, "uModelMtx");
    cameraMtxLoc = glGetUniformLocation(shaderProgram, "uCameraMtx");
    projectionMtxLoc = glGetUniformLocation(shaderProgram, "uProjectionMtx");

    normalMtxLoc = glGetUniformLocation(shaderProgram, "uNormalMtx");

    glUniformMatrix4fv(
        modelMtxLoc,
        1,
        GL_FALSE,
        modelMtx.data());
    glUniformMatrix4fv(
        cameraMtxLoc,
        1,
        GL_FALSE,
        cameraMtx.data());
    glUniformMatrix4fv(
        projectionMtxLoc,
        1,
        GL_FALSE,
        projectionMtx.data());

    glUniformMatrix4fv(
        normalMtxLoc,
        1,
        GL_FALSE,
        normalMtx.data());

    cameraPosLoc = glGetUniformLocation(shaderProgram, "uCameraPos");
    lightPosLoc = glGetUniformLocation(shaderProgram, "uLightPos");

    lightColorLoc = glGetUniformLocation(shaderProgram, "uLightColor");
    objectColorLoc = glGetUniformLocation(shaderProgram, "uObjectColor");

    glUniform3fv(
        cameraPosLoc,
        1,
        cameraPosition);
    glUniform3fv(
        lightPosLoc,
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

float t = 0.0f;
float dt = 0.025f;
void tick(int) {
    glutPostRedisplay();
    glutTimerFunc(16, tick, 0);
}

void displayFunc() {
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);

    initUniforms(t);

    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    t += dt;

    glutSwapBuffers();
}
