#include <MySimpleGE/Renderer/OpenGLRenderer.h>
#include <MySimpleGE/Renderer/GLTexture2d.h>
#include <MySimpleGE/Renderer/GLSLShader.h>
#include <MySimpleGE/Renderer/GLResource.h>
#include <MySimpleGE/Renderer/GLStaticModelRenderRequest.h>
#include <MySimpleGE/Core/Mesh.h>
#include <MySimpleGE/Core/Resources/Material.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_inverse.hpp>

namespace MSGE 
{

const glm::mat4 OpenGLRenderer::ZUpMatrix = glm::rotate(glm::mat4(1.0),  glm::radians(-90.0f), glm::vec3(1.0,0.0,0.0));

OpenGLRenderer::OpenGLRenderer()
{

}

OpenGLRenderer::~OpenGLRenderer()
{
    std::cout << "GL Resource Map count ref: " << _glResourceManager.getCacheMap().size() << std::endl;
}

bool OpenGLRenderer::hasGLResource(const std::string& id)
{
    return _glResourceManager.hasRef(id);
}

int OpenGLRenderer::init(GLADloadproc glLoader) 
{
    if (!gladLoadGLLoader(glLoader))
        return 1;
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);  
    glEnable(GL_CULL_FACE);
    return 0;
}

void OpenGLRenderer::emptyRenderList()
{   
    _renderList.clear();
}

void OpenGLRenderer::printResources()
{
    for (auto it : _glResourceManager.getCacheMap())
    {
        std::cout << "GL Reference: " << it.first << ", ref count" << it.second.use_count() << std::endl;
    }
}

void OpenGLRenderer::addToRenderList(std::shared_ptr<GLStaticMeshRenderRequest> renderRequest)
{
    return _renderList.push_back(renderRequest);
}

void OpenGLRenderer::render() 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto req : _renderList)
    {
        auto shader = req->shader;
        auto meshBuffer = req->meshBuffer;

        shader->bind();
        shader->setMat4Uniform("modelMat", ZUpMatrix * req->modelMatrix);
        shader->setMat4Uniform("viewMat", viewMatrix);
        shader->setMat4Uniform("projMat", projectionMatrix);
        shader->setVec2Uniform("lightmapTiling", req->lightMapUVTiling);
        shader->setVec2Uniform("lightmapOffset", req->lightMapUVOffset);
        //shader->setVec3Uniform("lightPoint", glm::inverse(viewMatrix) * glm::vec4(0.0,0.0,0.0,1.0) );
        
        //Bind textures
        int slot = 0;
        for (const auto& textureUniform : req->texturesUniforms)
        {
            auto glTextureRes = textureUniform.glTexture;
            if (glTextureRes)
            {
                glActiveTexture(GL_TEXTURE0+slot);
                glTextureRes->bind();
                shader->setTextureUniform(textureUniform.uniformName, slot);
            }
            slot++;
        }

        meshBuffer->bind();

        glDrawElements(
            GL_TRIANGLES,      // mode
            meshBuffer->getIndexCount(),    // count
            GL_UNSIGNED_INT,   // type
            0          // element array buffer offset
        );

        shader->unbind();
        meshBuffer->unbind();

        //unbind textures
        for (const auto& textureUniform : req->texturesUniforms)
        {
            auto glTextureRes = textureUniform.glTexture;
            if (glTextureRes)
            {
                glTextureRes->unbind();
            }
        }
    }
}

}
