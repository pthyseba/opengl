#include "Shader.h"

#include <exception>
#include <sstream>


namespace Gfx {

Shader::Shader()
  : Shader(KDefaultVertexShader, KDefaultFragmentShader)
{}

Shader::Shader(const char* aVertexShader, const char* aFragmentShader)
{
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &aVertexShader, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
	std::stringstream errorMessage;
	errorMessage << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	throw std::runtime_error(errorMessage.str());
    }
    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &aFragmentShader, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::stringstream errorMessage;
        errorMessage << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	throw std::runtime_error(errorMessage.str());
    }
    // link shaders
    iShaderProgram = glCreateProgram();
    glAttachShader(iShaderProgram, vertexShader);
    glAttachShader(iShaderProgram, fragmentShader);
    glLinkProgram(iShaderProgram);
    // check for linking errors
    glGetProgramiv(iShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(iShaderProgram, 512, NULL, infoLog);
	std::stringstream errorMessage;
        errorMessage << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	throw std::runtime_error(errorMessage.str());
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::Activate() const
{
  glUseProgram(iShaderProgram);
}

}
