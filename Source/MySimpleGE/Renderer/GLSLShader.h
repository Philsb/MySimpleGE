#pragma once
#include <string>
#include <glad/glad.h>

namespace MSGE 
{

enum class GLSLShaderStatus 
{
    SHADER_VALID,
    SHADER_COMPILE_FAILURE,
    SHADER_NOT_INITIALIZED
};

class GLSLShader 
{
public:
    GLSLShader();
    void createShader(const std::string& vShaderSource, const std::string& fShaderSource);
    GLSLShaderStatus getStatus(){return _status;};
    void useShader();
    ~GLSLShader();
private:
    GLuint _vertexShader;
    GLuint _fragmentShader;
    GLuint _shaderProgram;
    GLSLShaderStatus _status;
};


}