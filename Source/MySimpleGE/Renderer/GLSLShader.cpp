#include <MySimpleGE/Renderer/GLSLShader.h>
#include <fstream>
#include <iostream>

namespace MSGE
{


/*GLSLShader::GLSLShader(std::string vShaderPath, std::string fShaderPath)
{
    std::ifstream vShaderFile(RESOURCES_PATH + vShaderPath);
    std::ifstream fShaderFile(RESOURCES_PATH + fShaderPath);

    if () 
    
}*/

GLSLShader::GLSLShader()
:
_status(GLSLShaderStatus::SHADER_NOT_INITIALIZED)
{
    
}

void GLSLShader::createShader(const std::string& vShaderSource, const std::string& fShaderSource)
{
    _vertexShader = glCreateShader(GL_VERTEX_SHADER);
    auto vShaderSource_c = vShaderSource.c_str();
    glShaderSource(_vertexShader, 1, &vShaderSource_c, NULL);
    glCompileShader(_vertexShader);

    GLint status;
    char infoLog[512];
    glGetShaderiv(_vertexShader, GL_COMPILE_STATUS, &status);
    if(!status)
    {
        glGetShaderInfoLog(_vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR:: SHADER VERTEX COMPILATION_FAILED\n" << infoLog << std::endl;
        _status = GLSLShaderStatus::SHADER_COMPILE_FAILURE;
        return;
    }

    _fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    auto fShaderSource_c = vShaderSource.c_str();
    glShaderSource(_fragmentShader, 1, &fShaderSource_c, NULL);
    glCompileShader(_fragmentShader);

    glGetShaderiv(_fragmentShader, GL_COMPILE_STATUS, &status);
    if(!status)
    {
        glGetShaderInfoLog(_fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR:: SHADER FRAGMENT COMPILATION_FAILED\n" << infoLog << std::endl;
        _status = GLSLShaderStatus::SHADER_COMPILE_FAILURE;
        return;
    }

    _shaderProgram = glCreateProgram();

    glAttachShader(_shaderProgram, _vertexShader);
    glAttachShader(_shaderProgram, _fragmentShader);
    glLinkProgram(_shaderProgram);

    glGetProgramiv(_shaderProgram, GL_LINK_STATUS, &status);
    if(!status) {
        glGetProgramInfoLog(_shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR:: SHADER PROGRAM FAILED TO COMPILE\n" << infoLog << std::endl;
        _status = GLSLShaderStatus::SHADER_COMPILE_FAILURE;
        return;
    }

    _status = GLSLShaderStatus::SHADER_VALID;

}

void GLSLShader::useShader()
{
    glUseProgram(_shaderProgram);
}

GLSLShader::~GLSLShader()
{
    glDeleteShader(_vertexShader);
    glDeleteShader(_fragmentShader);
    glDeleteProgram(_shaderProgram);
}

}