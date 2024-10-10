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
:IGLResource(),
_isBound(false),
_status(GLSLShaderStatus::SHADER_NOT_INITIALIZED)
{
    create();
}

GLSLShader::~GLSLShader()
{
    //if (_shaderProgram > 0)
    glDeleteProgram(_shaderProgram);
}

void GLSLShader::create()
{
    _shaderProgram = glCreateProgram();
}

void GLSLShader::setAndCompileShader(const std::string& vShaderSource, const std::string& fShaderSource)
{

    GLuint _vertexShader = glCreateShader(GL_VERTEX_SHADER);
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

    GLuint _fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    auto fShaderSource_c = fShaderSource.c_str();
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

    glDeleteShader(_vertexShader);
    glDeleteShader(_fragmentShader);
    _status = GLSLShaderStatus::SHADER_VALID;

}

void GLSLShader::bind()
{
    glUseProgram(_shaderProgram);
    _isBound = true;
}

void GLSLShader::unbind()
{
    glUseProgram(0);
    _isBound = false;
}

void GLSLShader::setMat4Uniform(const std::string& uniformName, glm::mat4 matrix)
{
    if (!_isBound)
        return;
    //This is a bit of overhead instead of caching uniform locations, but it lets us be more generic
    int uniformLoc = glGetUniformLocation(_shaderProgram, uniformName.c_str());
    glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, &matrix[0][0]);
}

void GLSLShader::setVec3Uniform(const std::string& uniformName, glm::vec3 vector)
{
    if (!_isBound)
        return;
    //This is a bit of overhead instead of caching uniform locations, but it lets us be more generic
    int uniformLoc = glGetUniformLocation(_shaderProgram, uniformName.c_str());
    glUniform3fv(uniformLoc, 1, &vector[0]);
}

void GLSLShader::setVec2Uniform(const std::string& uniformName, glm::vec2 vector)
{
    if (!_isBound)
        return;
    //This is a bit of overhead instead of caching uniform locations, but it lets us be more generic
    int uniformLoc = glGetUniformLocation(_shaderProgram, uniformName.c_str());
    glUniform2fv(uniformLoc, 1, &vector[0]);
}

void GLSLShader::setTextureUniform(const std::string& uniformName, GLint textureId)
{
    if (!_isBound)
        return;
    //TODO: Should we asume that is binded?
    //This is a bit of overhead instead of caching uniform locations, but it lets us be more generic
    GLint uniformLoc = glGetUniformLocation(_shaderProgram, uniformName.c_str());
    glUniform1i(uniformLoc, textureId);

}

}