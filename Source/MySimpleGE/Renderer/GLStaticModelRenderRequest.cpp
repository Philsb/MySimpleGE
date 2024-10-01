#include <MySimpleGE/Renderer/GLStaticModelRenderRequest.h>
#include <MySimpleGE/Core/Mesh.h>
#include <MySimpleGE/Renderer/GLTexture2d.h>
#include <iostream>

namespace MSGE
{

GLStaticModelRenderRequest::GLStaticModelRenderRequest()
: _modelMatrix(glm::mat4(1.0f))
{}
GLStaticModelRenderRequest::~GLStaticModelRenderRequest()
{
    std::cout << "Deleting Request" << std::endl;
}

void GLStaticModelRenderRequest::setMesh(std::shared_ptr<GLMeshBuffer> meshBuffer)
{
    _meshBuffer = meshBuffer;
}
void GLStaticModelRenderRequest::setShader(std::shared_ptr<GLSLShader> shader)
{
    _shader = shader;
}

void GLStaticModelRenderRequest::setModelMatrix(glm::mat4 matrix)
{
    _modelMatrix = matrix;
}
void GLStaticModelRenderRequest::setTextureUniform(const std::string& uniformName, unsigned int textureSlot, std::shared_ptr<GLTexture2d> texture)
{
    if (textureSlot < 16)
    {
        _textureUniforms[textureSlot] = {true , uniformName, texture};
    }
    else 
    {
        std::cerr << "Error: Texture incorrect slot, texure not bound" << textureSlot << "\n";
    }
}
void GLStaticModelRenderRequest::setFloatUniform(const std::string& uniformName, float value)
{   
    addVecVariantToMap(uniformName, {value});
}
void GLStaticModelRenderRequest::setVec2Uniform(const std::string& uniformName, glm::vec2 value)
{
    addVecVariantToMap(uniformName, {value});
}
void GLStaticModelRenderRequest::setVec3Uniform(const std::string& uniformName, glm::vec3 value)
{
    addVecVariantToMap(uniformName, {value});
}
void GLStaticModelRenderRequest::setVec4Uniform(const std::string& uniformName, glm::vec4 value)
{
    addVecVariantToMap(uniformName, {value});
}   
void GLStaticModelRenderRequest::setMat2Uniform(const std::string& uniformName, glm::mat2 value)
{
    addMatVariantToMap(uniformName, {value});
}
void GLStaticModelRenderRequest::setMat3Uniform(const std::string& uniformName, glm::mat3 value)
{
    addMatVariantToMap(uniformName, {value});
}
void GLStaticModelRenderRequest::setMat4Uniform(const std::string& uniformName, glm::mat4 value)
{
    addMatVariantToMap(uniformName, {value});
}

}