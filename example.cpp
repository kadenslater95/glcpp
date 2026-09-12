#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>


void displayFunc();


void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);

    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(4.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 500.0, 0.0, 500.0);
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 150);
    glutCreateWindow("glcpp");

    glutDisplayFunc(displayFunc);

    init();

    glutMainLoop();

    return 0;
}


void displayFunc() {
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_LINES);
        glVertex2i(100, 50);
        glVertex2i(100, 130);
    glEnd();

    glFlush();
}