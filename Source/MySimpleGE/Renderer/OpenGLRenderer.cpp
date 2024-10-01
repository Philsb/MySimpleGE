#include <MySimpleGE/Renderer/OpenGLRenderer.h>
#include <MySimpleGE/Renderer/GLTexture2d.h>
#include <MySimpleGE/Renderer/GLResource.h>
#include <MySimpleGE/Renderer/GLStaticModelRenderRequest.h>
#include <MySimpleGE/Core/Mesh.h>
#include <glm/gtc/matrix_transform.hpp>
namespace MSGE 
{
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

void OpenGLRenderer::addToRenderList(std::shared_ptr<GLStaticModelRenderRequest> renderRequest)
{
    return _renderList.push_back(renderRequest);
}

void OpenGLRenderer::render() 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto req : _renderList)
    {
        glm::mat4 projection = glm::perspective(glm::radians(90.0f), 4.0f/4.0f, 0.01f, 1000.0f);


        
        auto shader = req->getShader();
        auto meshBuffer = req->getMesh();

        shader->bind();
        shader->setMat4Uniform("modelMat", req->getModelMatrix());
        shader->setMat4Uniform("projMat", projection);
        
        //Bind textures
        int slot = 0;
        for (auto textureDesc : req->_textureUniforms)
        {
            if (textureDesc.valid)
            {
                glActiveTexture(GL_TEXTURE0+slot);
                textureDesc.texture->bind();
                shader->setTextureUniform(textureDesc.uniformName, slot);
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
    }
}

}
