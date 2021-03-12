#include <iostream>
#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>

void circle()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    int number = 40;
    float radius = 0.4f;
    float twopi = 2.0 * 3.1452423;
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glBegin(GL_POLYGON);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2f(0.0f, 0.0f);
    for(int i = 0; i <= 40; i++)
    {
        glVertex2f(radius*cosf(i * twopi/number), radius*sinf(i * twopi/number));
    }
    glEnd();
    glFlush();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Assignment 2 - Computer Graphic");
    glutDisplayFunc(circle);
    glutMainLoop();
    return 0;
}
