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

// Utils
#include "racani_utils.hh"

class Bspline
{
public:
    Bspline(std::string&& fileToControlPoints = "");

    void drawBspline();

    static void drawAll(float time = 0.);

    void drawControlPoints();

    static std::vector<std::unique_ptr<Bspline>> splines;

    VertexOnRoids getPositionOnCurve(float t);

private:


    glm::mat4x4 cubicBspline;
    float cubicBsplineFactor; 

    std::vector<glm::vec3> controlPoints;
    int numberOfControlPoints;

};