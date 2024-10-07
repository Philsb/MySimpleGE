#include <MySimpleGE/Core/Components/GLStaticMeshComponent.h>
#include <MySimpleGE/Core/Resources/ResourceManager.h>
#include <MySimpleGE/Core/Resources/MeshResource.h>
#include <MySimpleGE/Core/Resources/Texture2dResource.h>
#include <MySimpleGE/Core/Resources/Material.h>
#include <MySimpleGE/Core/Singleton.h>

#include <MySimpleGE/Renderer/OpenGLRenderer.h>
#include <MySimpleGE/Renderer/GLMeshBuffer.h>
#include <MySimpleGE/Renderer/GLTexture2d.h>
#include <MySimpleGE/Renderer/GLSLShader.h>
#include <MySimpleGE/Renderer/GLStaticModelRenderRequest.h>


namespace MSGE
{
GLStaticMeshComponent::GLStaticMeshComponent(OpenGLRenderer* renderer, 
                                            ResourcePath staticMeshPath, const std::vector<std::shared_ptr<Material>>& newMaterials)
:_renderer(renderer),
_staticMeshPath(staticMeshPath),
_materials(newMaterials)
{
    setup();
}
    
void GLStaticMeshComponent::setup()
{
    //Load mesh once and then discard,

    auto resManager = Singleton<ResourceManager>::getInstance();
    auto meshRes = resManager->load<MeshResource>(_staticMeshPath);
    if (meshRes)
    {
        //We need to make sure there are as many materials as mesh segments
        if (_materials.size() < meshRes->getMeshData().getSegments().size())
        {
            std::cerr << "ERROR: There less materials than mesh segments\n";
            return;
        }

        int meshIdx = 0;
        for (auto meshSegment : meshRes->getMeshData().getSegments())
        {
            //Create Request
            auto renderReq = std::make_shared<GLStaticMeshRenderRequest>();
            
            //Creates the opengl meshBuffer 
            auto glMeshHandleStr = _staticMeshPath.getRelativePath() + ".seg" + std::to_string(meshIdx);
            auto meshBuffer = _renderer->getGLResource<GLMeshBuffer>(glMeshHandleStr);
            if (!meshBuffer)
            {
                meshBuffer = _renderer->allocateGLResource<GLMeshBuffer>(glMeshHandleStr);
                meshBuffer->setData(meshSegment.vertices, meshSegment.indices);   
            }

            auto shaderPath = _materials[meshIdx]->getShaderPath();
            auto shaderRef = _renderer->getGLResource<GLSLShader>( shaderPath.getRelativePath() );
            //If shader not loaded we need to load it
            if(!shaderRef)
            {
                ShaderSource ss = _materials[meshIdx]->getShaderSource();
                shaderRef = _renderer->allocateGLResource<GLSLShader>( shaderPath.getRelativePath() );
                shaderRef->setAndCompileShader(ss.vShaderSource, ss.fShaderSource);
            }

            //Uniforms
            for (TextureParam texParam : _materials[meshIdx]->getTextureParams())
            {
                //For each texture load or get its gl resource counterpart and then add to req
                auto glTexRef = _renderer->getGLResource<GLTexture2d>( texParam.texturePath.getRelativePath() );
                if (!glTexRef)
                {
                    glTexRef = _renderer->allocateGLResource<GLTexture2d>( texParam.texturePath.getRelativePath() );
                    auto texRes = resManager->load<Texture2dResource>(texParam.texturePath);
	                if (texRes)
                    {
                        glTexRef->setData(texRes->getWidth(), texRes->getHeight(), texRes->getImageData());              
                    }
                }

                renderReq->texturesUniforms.push_back({ texParam.paramName, glTexRef});

            }

            renderReq->modelMatrix = this->modelMatrix;
            renderReq->meshBuffer = meshBuffer;
            renderReq->shader = shaderRef;

            meshIdx++;

            _renderRequests.push_back(renderReq);
        }
    }
}

void GLStaticMeshComponent::updateRequests()
{
    for(auto request : _renderRequests)
    {
        request->modelMatrix = modelMatrix;
    }
}

void GLStaticMeshComponent::addRequestsToRenderList()
{
    for(auto request : _renderRequests)
    {
        _renderer->addToRenderList(request);
    }
}
} // namespace MSGE

