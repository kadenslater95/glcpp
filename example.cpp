#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <GL/glew.h>
#include <GL/glut.h>

#include <Eigen/Dense>


GLuint shaderProgram, VAO;
GLuint VBOs[2];
GLint modelLoc, cameraLoc, projectionLoc;
GLint normalMatrixLoc;

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

    if(glewInit() != GLEW_OK) {
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

    if(!shader_fstream.is_open()) {
        std::cerr << "Failed to open shader source file!" << "\n";
        return 0; // Note: glCreateShader returns 0 for error
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

    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, VBOs);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void *) 0);
    glEnableVertexAttribArray(1);

    // Unselect any vertex buffer object or attribute pointer array (they start at index 1) as the final cleanup
    // of initialization so we don't accidentally set something somewhere else unexpectedly
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
        . . .
    );
}


void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);

    GLuint vertexShader = compileShader("shaders/perspective.vert", GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader("shaders/blinn_phong.frag", GL_FRAGMENT_SHADER);

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