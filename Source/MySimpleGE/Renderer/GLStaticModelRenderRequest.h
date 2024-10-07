#pragma once
#include<glm/glm.hpp>
#include<MySimpleGE/Renderer/GLMeshBuffer.h>
#include<memory>
#include<map>
#include<string>


namespace MSGE
{

class GLSLShader;
class GLTexture2d;
class Material;

struct GLTextureUniform
{
    std::string uniformName;
    std::shared_ptr<GLTexture2d> glTexture;
};

struct GLStaticMeshRenderRequest
{
    glm::mat4 modelMatrix = glm::mat4(1.0);
    std::shared_ptr<GLMeshBuffer> meshBuffer;
    std::shared_ptr<GLSLShader> shader;
    std::vector<GLTextureUniform> texturesUniforms;
};
}