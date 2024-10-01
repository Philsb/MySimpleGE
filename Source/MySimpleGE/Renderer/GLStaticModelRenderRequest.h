#pragma once
#include<MySimpleGE/Renderer/GLMeshBuffer.h>
#include<MySimpleGE/Renderer/GLSLShader.h>
#include<glm/glm.hpp>
#include<memory>
#include<map>
#include<string>
#include<variant>
#include<array>

namespace MSGE
{
class GLTexture2d;

struct GLUniformVariantVec
{
    std::variant <float, glm::vec2, glm::vec3, glm::vec4> uniform;
};

struct GLUniformVariantMat
{
    std::variant <glm::mat2, glm::mat3, glm::mat4> uniform;
};

struct GLTextureUniformDescriptor
{
    //valid is used to verify that the array of texture uniforms is able to be used
    bool valid = false;
    std::string uniformName = "";
    std::shared_ptr<GLTexture2d> texture;
};

class GLStaticModelRenderRequest
{
    friend class OpenGLRenderer;
public:
    GLStaticModelRenderRequest();
    ~GLStaticModelRenderRequest();

    std::shared_ptr<GLMeshBuffer> getMesh() const {return _meshBuffer;}
    std::shared_ptr<GLSLShader> getShader() const {return _shader;}
    glm::mat4 getModelMatrix() const {return _modelMatrix;}

    void setMesh(std::shared_ptr<GLMeshBuffer> mesh);
    void setShader(std::shared_ptr<GLSLShader> shader);
    void setModelMatrix(glm::mat4 matrix);
    void setTextureUniform(const std::string& uniformName, unsigned int textureSlot, std::shared_ptr<GLTexture2d> texture);
    void setFloatUniform(const std::string& uniformName, float value);
    void setVec2Uniform(const std::string& uniformName, glm::vec2 value);
    void setVec3Uniform(const std::string& uniformName, glm::vec3 value);
    void setVec4Uniform(const std::string& uniformName, glm::vec4 value);
    void setMat2Uniform(const std::string& uniformName, glm::mat2 value);
    void setMat3Uniform(const std::string& uniformName, glm::mat3 value);
    void setMat4Uniform(const std::string& uniformName, glm::mat4 value);

private:

    glm::mat4 _modelMatrix;
    std::shared_ptr<GLMeshBuffer> _meshBuffer;
    std::shared_ptr<GLSLShader> _shader;

    //TODO: what about double precision floats?
    //TODO: maybe we want bool, and integer versions?
    //TODO: Uniform buffers?
    std::map<std::string, GLUniformVariantVec> _vecUniforms;
    std::map<std::string, GLUniformVariantMat> _matUniforms;
    std::array<GLTextureUniformDescriptor, 16> _textureUniforms;

    inline void addVecVariantToMap(const std::string& uniformName, GLUniformVariantVec vecVariant)
    {
        _vecUniforms.insert_or_assign(uniformName, vecVariant);
    } 

    inline void addMatVariantToMap(const std::string& uniformName, GLUniformVariantMat matVariant)
    {
        _matUniforms.insert_or_assign(uniformName, matVariant);
    }     

};
}