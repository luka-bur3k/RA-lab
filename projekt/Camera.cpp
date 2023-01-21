#define GL_GLEXT_PROTOTYPES 1
#define GL3_PROTOTYPES 1

#include "Camera.h"
#include <glm/gtx/quaternion.hpp>


#include <iostream>

Camera::Camera()
{
    _width = 640;
    _height = 480;

    _position = glm::vec3{0.,0.,2.};     // default position of camera
    _orientation = glm::vec3{0.,0.,-1.}; // default direction of camera
    _up = glm::vec3(0.,1.,0.);           // default up direction of camera

    _rotQuat = glm::quat(1,0,0,0);
    _rotQuatConj = glm::conjugate(_rotQuat);
}

void Camera::exportCameraToShader(GLfloat FOVdeg, GLfloat nearPlane, GLfloat farPlane, Shader &shader)
{
    glm::mat4 view = glm::lookAt(_position, _position + _orientation, _up);
    glm::mat4 projection = glm::perspective(glm::radians(FOVdeg), (float)_width / (float)_height, nearPlane, farPlane);

    //glUniformMatrix4fv(glGetUniformLocation(shader.getProgramID(), "viewMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
    //glUniformMatrix4fv(glGetUniformLocation(shader.getProgramID(), "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(view));
    glUniform3f(glGetUniformLocation(shader.getProgramID(), "camPosition"), _position.x, _position.y, _position.z);
    glUniform4f(glGetUniformLocation(shader.getProgramID(), "camRotationQuaternion"), _rotQuat.x, _rotQuat.y, _rotQuat.z, _rotQuat.w);
    glUniform4f(glGetUniformLocation(shader.getProgramID(), "camRotationQuaternionConjugate"), _rotQuatConj.x, _rotQuatConj.y, _rotQuatConj.z, _rotQuatConj.w);
}

void Camera::handleInputs(const unsigned char key)
{
    std::cerr << "CamInfo :\n";

    std::cerr << "CamPos(" << _position.x << "," << 
        _position.y << "," <<
        _position.z << ")\n";

    std::cerr << "CamOri(" << _orientation.x << "," << 
        _orientation.y << "," <<
        _orientation.z << ")\n";

    std::cerr << "-------------------\n";

    glm::quat rotationQuaternion;

    switch (key)
    {
    case 'w':
        _position += 0.01f * glm::normalize(_orientation);
        break;
    case 'a':
        _position += 0.01f * glm::normalize(glm::cross(_up, _orientation));        
        break;
    case 's':
        _position -= 0.01f * glm::normalize(_orientation);        
        break;
    case 'd':
        _position += 0.01f * glm::normalize(glm::cross(_orientation, _up));        
        break;
    case 'u':
        rotationQuaternion = glm::angleAxis(glm::radians(-0.5f), glm::cross(_up, _orientation));
        _rotQuat = rotationQuaternion * _rotQuat;
        //_rotQuatConj *= glm::conjugate(rotationQuaternion);
        _orientation = glm::rotate(_rotQuat, glm::vec3(0,0,-1));
        _up = glm::rotate(_rotQuat, glm::vec3(0,1,0));
        break;
    case 'h':
        rotationQuaternion = glm::angleAxis(glm::radians(+0.5f), _up);
        _rotQuat = rotationQuaternion * _rotQuat;
        //_rotQuatConj *= glm::conjugate(rotationQuaternion);
        _orientation = glm::rotate(_rotQuat, glm::vec3(0,0,-1));
        _up = glm::rotate(_rotQuat, glm::vec3(0,1,0));
        break;
    case 'j':
        rotationQuaternion = glm::angleAxis(glm::radians(+0.5f), glm::cross(_up, _orientation));
        _rotQuat = rotationQuaternion * _rotQuat;
        //_rotQuatConj *= glm::conjugate(rotationQuaternion);
        _orientation = glm::rotate(_rotQuat, glm::vec3(0,0,-1));
        _up = glm::rotate(_rotQuat, glm::vec3(0,1,0));
        break;
    case 'k':
        rotationQuaternion = glm::angleAxis(glm::radians(-0.5f), _up);
        _rotQuat = rotationQuaternion * _rotQuat;
        //_rotQuatConj *= glm::conjugate(rotationQuaternion);
        _orientation = glm::rotate(_rotQuat, glm::vec3(0,0,-1));
        _up = glm::rotate(_rotQuat, glm::vec3(0,1,0));
        break;
    default:
        break;
    }

    _orientation = glm::normalize(_orientation);
    _up = glm::normalize(_up);
}
