#pragma once

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


struct Particle 
{
    glm::vec3 position;
    GLfloat spawnedTime;

    Particle(glm::vec3 pos, GLfloat inGameTime);
};

class ParticleSystem
{
    std::vector<Particle> particles;
    std::vector<GLfloat> buffer;

public :
    ParticleSystem(int numberOfParticles = 100);
    std::vector<GLfloat> getBuffer();

    void update(GLfloat inGameTime, glm::vec3 ociste);
};