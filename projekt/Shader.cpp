#include <sstream>
#include <fstream>

#include <vector>

#define GL_GLEXT_PROTOTYPES 1
#define GL3_PROTOTYPES 1

#include "Shader.h"


/**
 * @brief read file into string
 * 
 * @param filePath      file to be read
 * @return std::string  file contents 
 */
std::string readFile(const char* filePath)
{
    std::string code;

    std::ifstream fileStream(filePath, std::ios::in);
	if(fileStream.is_open())
    {
		std::stringstream sstr;
		sstr << fileStream.rdbuf();
		code = sstr.str();
		fileStream.close();
	}
    else
    {
		printf("Impossible to open %s. Are you in the right directory ?\n", filePath);
		getchar();
		return std::string{};
	}
    return code;
}

bool compileAndCheckShader(const char* filePath, std::string& code, GLuint shaderID)
{
    // Compile Vertex Shader
    GLint result = GL_FALSE;
    int infoLogLength;

	printf("Compiling shader : %s\n", filePath);

	char const * sourcePointer = code.c_str();
	glShaderSource(shaderID, 1, &sourcePointer , NULL);
	glCompileShader(shaderID);

	// Check Vertex Shader
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if ( infoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(infoLogLength+1);
		glGetShaderInfoLog(shaderID, infoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

    return result;
}

Shader::Shader()
{
    // empty
}

void Shader::load(const char *vertexShaderFilePath, const char *fragmentShaderFilePath)
{
    // Create shaders
    _vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    _fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    std::string vertexShaderCode{readFile(vertexShaderFilePath)};
    std::string fragmentShaderCode{readFile(fragmentShaderFilePath)};
    
    assert(compileAndCheckShader(vertexShaderFilePath, vertexShaderCode, _vertexShaderID));
    assert(compileAndCheckShader(fragmentShaderFilePath, fragmentShaderCode, _fragmentShaderID));

    GLint result = GL_FALSE;
    int infoLogLength;

    // Link the program
	printf("Linking program\n");
	_programID = glCreateProgram();
	glAttachShader(_programID, _vertexShaderID);
	glAttachShader(_programID, _fragmentShaderID);
	glLinkProgram(_programID);

	// Check the program
	glGetProgramiv(_programID, GL_LINK_STATUS, &result);
	glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if ( infoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(infoLogLength+1);
		glGetProgramInfoLog(_programID, infoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}
	
	glDetachShader(_programID, _vertexShaderID);
	glDetachShader(_programID, _fragmentShaderID);
	
	glDeleteShader(_vertexShaderID);
	glDeleteShader(_fragmentShaderID);
}

Shader::~Shader()
{
    deactivate();
    glDeleteProgram(_programID);
}

void Shader::activate()
{
    glUseProgram(_programID);
}

void Shader::deactivate()
{
    glUseProgram(0);
}

GLuint Shader::getProgramID()
{
    return _programID;
}
