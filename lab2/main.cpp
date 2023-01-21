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

#include "shaderLoader.h"

// my headers
#include "particle.h"

// function definitions
void display();
void reshape(int width, int height);
void renderScene();
void idle();
void keyboard(unsigned char theKey, int mouseX, int mouseY);




glm::vec3 ociste = {0.0f, 0.0f, 1.0f};


// starting data
GLuint width = 600, height = 600;
GLfloat inGameTime = 0.0;

GLuint VertexArrayID;
GLuint vertexBuffer;
GLuint programID;

ParticleSystem *s_ptr;

std::vector<Particle> particles;

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("lab_2");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);

    // cemu ovo ?
    //glGenVertexArrays(1, &VertexArrayID);
    //glBindVertexArray(VertexArrayID);

    // add all particles
    ParticleSystem s(25);
    s_ptr = &s;

    // VBO
    glGenBuffers(1, &vertexBuffer);

    programID = LoadShaders( "./lab2/vert.vs", "./lab2/frag.fs" );

    glEnable(GL_DEPTH_TEST); // zasto depth filter ne radi ? 

    glutMainLoop();
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
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    s_ptr->update(inGameTime, ociste);
    auto particleBuffer = s_ptr->getBuffer();

    glBufferData(GL_ARRAY_BUFFER, particleBuffer.size() * 4, &particleBuffer[0], GL_DYNAMIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,        // attribute. No particular reason for 0, but must match the layout in the shader.
        3,        // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        6 * sizeof(float),        // stride
        (void *)0 // array buffer offset
    );
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,        // attribute. fo particular reason for 0, but must match the layout in the shader.
        3,        // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        6 * sizeof(float),          // stride
        (void *)(3 * sizeof(float)) // array buffer offset
    );
    glUseProgram(programID);
    glDrawArrays(GL_TRIANGLES, 0, particleBuffer.size() / 6);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

}


void keyboard(unsigned char theKey, int mouseX, int mouseY)
{
    switch (theKey)
    {
    case 'a':
        ociste.x = ociste.x + 0.2f;
        break;
    case 'd':
        ociste.x = ociste.x - 0.2f;
        break;
    case 'w':
        ociste.y = ociste.y + 0.2f;
        break;
    case 's':
        ociste.y = ociste.y - 0.2f;
        break;
    case 'r':
        ociste.x = 0.0;
        ociste.y = 0.0;
        break;
    case 27:
        exit(0);
        break;
    }

    reshape(width, height);
    glutPostRedisplay();
}

void idle()
{
    inGameTime += 0.01;
    glutPostRedisplay();
}
