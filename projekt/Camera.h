#pragma once 

#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

class Camera {

public:

    /**
     * @brief Construct a new Camera object
     */
    Camera();

    /**
     * @brief Export camera matrices to shader
     * 
     * @param FOVdeg    Fov degree of camera
     * @param nearPlane 
     * @param farPlane 
     * @param shader    Shader to which to export the matrices to 
     * @param uniform  
     */
    void exportCameraToShader(GLfloat FOVdeg, GLfloat nearPlane, GLfloat farPlane, Shader &shader);

    /**
     * @brief Handling inputs in keyboard function
     * 
     * (position changes)
     * W - move forward
     * A - move left
     * S - move backwards
     * D - move right
     * 
     * (orientation changes)
     * H - look left
     * U - look up
     * J - look down
     * K - look right
     * 
     * # this should also affect the up vector
     * 
     * @param key id of key pressed
     */
    void handleInputs(const unsigned char key);

private:

    glm::vec3 _position;    // position of camera in world space 
    glm::vec3 _orientation; // at what point is the camera looking at (world space)
    glm::vec3 _up;          // up direction of camera

    glm::quat _rotQuat;
    glm::quat _rotQuatConj;

    GLint _width;            // width of camera lens
    GLint _height;           // height of camera lens


};