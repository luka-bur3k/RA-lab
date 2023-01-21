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

// my includes
#include "mesh.hh"
#include "objLoader.hh"
#include "bspline.hh"

// function definitions
void display();
void reshape(int width, int height);
void renderScene();
void idle();
void keyboard(unsigned char theKey, int mouseX, int mouseY);

// starting data
GLuint width = 600, height = 600;

typedef struct _Ociste
{
    GLdouble x;
    GLdouble y;
    GLdouble z;
} Ociste;

float fake_time = 0;
Ociste ociste = {0.0f, 0.0f, 2.0f};

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("lab_1");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);

    loadOBJ("./test/teapot.obj", Mesh::meshes);
    Bspline::splines.emplace_back(new Bspline("./lab1/controlPoints.txt"));

    glutMainLoop();
    return 0;
}

//#########################################//

void display()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    renderScene();
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    width = w;
    height = h;
    // glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-5, 30, -5, 30, -10, 30);
    // glFrustum(-1, 1, -1, 1, 1, 5);
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(ociste.x, ociste.y, ociste.z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void draw()
{
    Mesh::drawAll(fake_time);
    Bspline::drawAll(fake_time);
}

void renderScene()
{
    draw();
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
    fake_time += 0.005;
    if (fake_time > 300.0)
        fake_time = 0;
    glutPostRedisplay();
}
