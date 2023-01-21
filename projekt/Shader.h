#pragma once 

#include <GL/glut.h>
#include <glm/glm.hpp>

class Shader {

public:

    /**
     * @brief Construct a new Shader object
     */
    Shader();

    /**
     * @brief Load all necessary files into shader
     * 
     * @param vertexShaderFilePath    file path to vertex shader 
     * @param fragmentShaderFilePath  file path to fragment shader
     */
    void load(const char* vertexShaderFilePath, const char* fragmentShaderFilePath);

    /**
     * @brief Destroy the Shader object
     */
    ~Shader();

    /**
     * @brief Activate this shader as current 
     */
    void activate();

    /**
     * @brief Deactivate this shader as current
     */
    void deactivate();

    /**
     * @brief Get the Program ID object
     * 
     * @return GLuint ID of program 
     */
    GLuint getProgramID();

private:

    GLuint _programID;          // program of this shader instance

    GLuint _vertexShaderID;     // vertex shader of this shader instance 
    GLuint _fragmentShaderID;   // fragment shader of this shader instance

};