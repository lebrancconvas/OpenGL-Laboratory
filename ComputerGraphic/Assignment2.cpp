#include <iostream>
#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;

const GLint WIDTH = 800, HEIGHT = 600;
const float toRadian = 3.14159265f / 180.0f;
float trioffset = 0.0f;
float triIncrease = 0.3f;
float trimaxoffset = 0.5f;
bool direction = true;
GLuint VAO, VBO, IBO, shader, uniformModel, uniformProjection, indexCount;

//Vertex Shader
static const char* vShader =
"                                                               \n\
#version 330                                                    \n\
                                                                \n\
layout (location = 0) in vec3 pos;                              \n\
                                                                \n\
out vec4 vCol;                                                  \n\
                                                                \n\
uniform mat4 model;                                             \n\
uniform mat4 projection;                                        \n\
                                                                \n\
void main() {                                                   \n\
    gl_Position = projection * model * vec4(pos, 1.0);          \n\
    vCol = vec4(1.0f, 0.0f, 0.0f, 1.0f);                  \n\
}                                                               \n\
";

//Fragment Shader
static const char* fShader =
"                                                               \n\
                                                                \n\
#version 330                                                    \n\
in vec4 vCol;                                                   \n\
out vec4 colour;                                                \n\
                                                                \n\
void main()                                                     \n\
{                                                               \n\
    colour = vCol;                                              \n\
}                                                               \n\
";

//Create Triangle
void CreateTriangle() {
    GLfloat vertices[] =
    {
        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        0.0f, 1.6f, 0.0f,
        1.0f, 1.6f, 0.0f
    };
    
    unsigned int indices[] =
    {
        0, 2, 3,
        0, 3, 1,
        2, 4, 5,
        2, 5, 3
    };
    
}



//Creat Shader
void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) {
    GLuint theShader = glCreateShader(shaderType);
    const GLchar* theCode[1];
    theCode[0] = shaderCode;
    
    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);
    
    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);
    
    GLint result = 0;
    GLchar elog[1024] = { 0 };
    
    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if(!result) {
        glGetShaderInfoLog(theShader, sizeof(elog), NULL, elog);
        printf("Error compiler the %d shader: '%s'\n", shaderType, elog);
        return;
    }
    glAttachShader(theProgram, theShader);
}

//Compile Shader
void CompileShaders() {
    shader = glCreateProgram();
    
    if(!shader) {
        printf("Error Creating Shader Program!\n");
        return;
    }
    
    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);
    
    GLint result = 0;
    GLchar elog[1024] = { 0 };
    
    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    
    if(!result) {
        glGetProgramInfoLog(shader, sizeof(elog), NULL, elog);
        printf("Error linking program: '%s'\n", elog);
        return;
    }
    
    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    
    if(!result) {
        glGetProgramInfoLog(shader, sizeof(elog), NULL, elog);
        printf("Error validating program : %s\n", elog);
        return;
    }
    
    uniformModel = glGetUniformLocation(shader, "model");
    uniformProjection = glGetUniformLocation(shader, "projection");
}

void useShader() {
    glUseProgram(shader);
}

//Animation Update
void update(long elapsedTime) {
    if(direction) {
        trioffset += triIncrease * (elapsedTime / 1000.0);
    } else {
        trioffset -= triIncrease * (elapsedTime / 1000.0);
    }
    
    if(direction && trioffset >= trimaxoffset) {
        direction = false;
    }
    if(!direction && trioffset <= -trimaxoffset) {
        direction = true;
    }
}

void renderMesh() {
    glBindVertexArray(VAO);
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices)
{
    indexCount = numOfIndices;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}



//Main Function
int main() {
    if(!glfwInit()) {
        printf("GLFW Installation Failed");
        glfwTerminate();
        return -1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "CG - Assignment 2", NULL, NULL);

    if(!mainWindow) {
        glfwTerminate();
        return -1;
    }
    
    int bufferWidth; int bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    glfwMakeContextCurrent(mainWindow);
    
    glewExperimental = GL_TRUE;
    
    if(glewInit() != GLEW_OK) {
        printf("GLEW Installation Failed.");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }
    
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, bufferWidth, bufferHeight);
    
    
    CreateTriangle();
    glBindVertexArray(VAO);
    CompileShaders();
    
    mat4 projection = perspective(45.0f, (GLfloat)bufferWidth / (GLfloat)bufferHeight, 0.1f, 100.0f);
    
    auto currentTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    
    long lastTime = currentTime;
    long elaspTime;
    
    
    
    while(!glfwWindowShouldClose(mainWindow)) {
        //Create Animation
        currentTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        
        elaspTime = currentTime - lastTime;
        lastTime = currentTime;
        
        //Get + Handle user input events
        glfwPollEvents();
        
        update(elaspTime);
        
        //Clear Window
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //Draw Here
        useShader();
        
        mat4 model (1.0f);
        /* Transformation */
        model = translate(model, vec3(trioffset, 0.0f, -2.5f));
        //model = rotate(model, 90.0f * toRadian, vec3(0.0f, 0.0f, 1.0f));
        model = scale(model, vec3(0.4f, 0.4f, 1.0f));
        
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, value_ptr(projection));
        
        renderMesh();
        
        glUseProgram(0);
        
        //end draw
        glfwSwapBuffers(mainWindow);
    }
    return 0;
}
