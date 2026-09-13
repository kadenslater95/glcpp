#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <GL/glew.h>
#include <GL/glut.h>


GLuint compileShader(const std::string, GLenum);
void initGeometry();
void init();

void displayFunc();


GLuint shaderProgram, VAO, VBO;


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
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);

    GLuint vertexShader = compileShader("shader.vert", GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader("shader.frag", GL_FRAGMENT_SHADER);

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