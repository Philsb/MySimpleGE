#pragma once
#include <string>
#include <glad/glad.h>
#include <MySimpleGE/Renderer/GLResource.h>
#include <glm/glm.hpp>

namespace MSGE 
{

enum class GLSLShaderStatus 
{
    SHADER_VALID,
    SHADER_COMPILE_FAILURE,
    SHADER_NOT_INITIALIZED
};
class GLSLShader : public IGLResource
{
public:
    GLSLShader();
    ~GLSLShader();

    void setAndCompileShader(const std::string& vShaderSource, const std::string& fShaderSource);
    GLSLShaderStatus getStatus(){return _status;};
    void create() override;
    void bind() override;
    void unbind() override;

    void setTextureUniform(const std::string& uniformName, GLint textureId);
    void setMat4Uniform(const std::string& uniformName, glm::mat4 matrix);
    void setVec3Uniform(const std::string& uniformName, glm::vec3 vector);
    void setVec2Uniform(const std::string& uniformName, glm::vec2 vector);
    
private:
    GLuint _shaderProgram;
    bool _isBound;
    GLSLShaderStatus _status;
};


}