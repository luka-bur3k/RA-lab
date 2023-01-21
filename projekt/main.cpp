#define GL_GLEXT_PROTOTYPES 1
#define GL3_PROTOTYPES 1

#include <GL/glut.h>
#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// C++ includes
#include <memory>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cstdlib>

#include "Camera.h"
#include "Shader.h"

// function definitions
void display();
void reshape(int width, int height);
void renderScene();
void idle();
void keyboard(unsigned char theKey, int mouseX, int mouseY);

GLuint VertexArrayID;
GLuint vertexBuffer;

Shader shader;
Camera camera;

GLuint width;
GLuint height;

static float timer = 0.;

GLfloat vertices[] = {
    -1., -1., 0.,
    1., 1., 0.,
    -1., 1., 0.,
    1., -1., 0.};

GLuint indices[] = {
    0, 1, 2,
    0, 3, 1};

GLuint VAO, VBO, EBO;

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(320, 240);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("PROJEKT");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    shader.load("./projekt/vert.vs", "./projekt/frag.fs");
    shader.activate();

    glEnable(GL_DEPTH_TEST);
    glutMainLoop();

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    return 0;
}

// #########################################//

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderScene();
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    width = w;
    height = h;
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

void renderScene()
{
    glBindVertexArray(VAO);
    camera.exportCameraToShader(45.0f, 0.1f, 100.f, shader);
    glUniform1f(glGetUniformLocation(shader.getProgramID(), "time"), timer);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void keyboard(unsigned char theKey, int mouseX, int mouseY)
{
    camera.handleInputs(theKey);

    reshape(width, height);
    glutPostRedisplay();
}

void idle()
{
    timer += 0.02;
    glutPostRedisplay();
}
