#include <stdio.h>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>

const GLint WIDTH = 800, HEIGHT = 600;

void drawCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides);
void display_bezier(void);
void bezier(int n);

int main()
{
    GLFWwindow *window;
    
    //Initialize the GLFW
    if(!glfwInit())
    {
        return -1;
    }
    
    //Create Windows mode and Its OpenGL Context
    window = glfwCreateWindow(WIDTH, HEIGHT, "Assignment 2 - CG", NULL, NULL);
    
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    
    
    if(!window)
    {
        glfwTerminate();
        return -1;
    }
    
    //Make the windows context current
    glfwMakeContextCurrent(window);
    
    //Draw Shape
    GLfloat vertices[] =
    {
       500.0f, 300.0f, 0.0f,
       500.0f, 420.0f, 0.0f,
       600.0f, 420.0f, 0.0f,
       600.0f, 300.0f, 0.0f
    };
    
    GLfloat vertices2[] =
    {
        500.0f, 400.0f, 0.0f,
        500.0f, 420.0f, 0.0f,
        600.0f, 420.0f, 0.0f,
        600.0f, 400.0f, 0.0f
    };
    
    GLfloat line_vertices[] =
    {
        600.0f, 400.0f, 0.0f,
        700.0f, 400.0f, 0.0f
    };
    
    GLfloat tri_vertices[] =
    {
        320, 240, 0,
        370, 290, 0,
        420, 240, 0
    };
    
    glViewport(0.0f, 0.0f, screenWidth, screenHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WIDTH, 0, HEIGHT, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    GLfloat colour[] =
    {
        0, 0, 0,
        0, 0, 0,
        0, 0, 0,
        0, 0, 0
    };
    
    GLfloat colour2[] =
    {
        255, 255, 255,
        255, 255, 255,
        255, 255, 255,
        255, 255, 255
    };
    
    //Loop Until the user closes the window
    while(!glfwWindowShouldClose(window))
    {
        glClearColor(1.0f, 0.56f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        //Render The OpenGL Here
        drawCircle(WIDTH / 2, HEIGHT / 2, 0, 120, 360);
        
        glfwSwapBuffers(window);
        
        //Poll for and process event
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}

void drawCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides)
{
    GLint numberOfVertices = numberOfSides + 2;
    GLfloat doublePi = 2.0f * M_PI;
    GLfloat circleVerticesX[numberOfVertices];
    GLfloat circleVerticesY[numberOfVertices];
    GLfloat circleVerticesZ[numberOfVertices];
    
    circleVerticesX[0] = x;
    circleVerticesY[0] = y;
    circleVerticesZ[0] = z;
    
    for(int i = 1; i < numberOfVertices; i++)
    {
        circleVerticesX[i] = x + (radius * cos(i * doublePi / numberOfSides));
        circleVerticesY[i] = y + (radius * sin(i * doublePi / numberOfSides));
        circleVerticesZ[i] = z;
    }
    
    GLfloat allCircleVertices[numberOfVertices * 3];
    
    for(int i = 0; i < numberOfVertices; i++)
    {
        allCircleVertices[i * 3] = circleVerticesX[i];
        allCircleVertices[(i * 3) + 1] = circleVerticesY[i];
        allCircleVertices[(i * 3) + 2] = circleVerticesZ[i];
    }
    
    GLfloat colour_circle[] =
    {
        0, 0, 0,
        0, 0, 0,
        0, 0, 0,
        0, 0, 0
    };
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnable(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, allCircleVertices);
    glColorPointer(120, GL_FLOAT, 0, colour_circle);
    glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisable(GL_VERTEX_ARRAY);
}

/*
void display_bezier(void) {
    if (count == 4) {
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < count; i++) {
            glColor3f(1.0f, 1.0f, 1.0f);//The color of the painted points
            glVertex3f(Points[i][0], Points[i][1], Points[i][2]);
        }
        glEnd();
        Bezier(20);
    }
    else {
        glBegin(GL_POINTS);
        for (int i = 0; i < count; i++) {
            glColor3f(1.0f, 1.0f, 1.0f);//The color of the painted points
            glVertex3f(Points[i][0], Points[i][1], Points[i][2]);
        }
        glEnd();
    }
}


void bezier(int n) {
    float f1, f2, f3, f4;
    float deltaT = 1.0 / n;
    float T;
    glBegin(GL_LINE_STRIP);
    glColor3f(0.8f, 0.2f, 0.5f);
    for (int i = 0; i <= n; i++) {
        T = i * deltaT;
        f1 = (1 - T) * (1 - T) * (1 - T);
        f2 = 3 * T * (1 - T) * (1 - T);
        f3 = 3 * T * T * (1 - T);
        f4 = T * T * T;
        float x = f1 * Points[0][0] + f2 * Points[1][0] + f3 * Points[2][0] + f4 * Points[3][0];
        float y = f1 * Points[0][1] + f2 * Points[1][1] + f3 * Points[2][1] + f4 * Points[3][1];
        float z = f1 * Points[0][2] + f2 * Points[1][2] + f3 * Points[2][2] + f4 * Points[3][2];
        glVertex3f(x, y, z);
    }
    glEnd();
}
*/
