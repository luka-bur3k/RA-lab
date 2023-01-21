#pragma once

#include <GL/glut.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// C++ includes
#include <memory>
#include <vector>
#include <iostream>

// Utils
#include "racani_utils.hh"

class Mesh
{
public:
    Mesh(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices);

    void draw();

    static void drawAll(float fake_time = 0.f);

    static std::vector<std::unique_ptr<Mesh>> meshes;

private:

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
};
