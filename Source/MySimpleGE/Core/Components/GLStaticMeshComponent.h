#pragma once
#include <MySimpleGE/Core/Resources/Resource.h>
#include<glm/glm.hpp>
#include <memory>
#include <vector>


namespace MSGE
{
class GLSceneRenderer;
class GLStaticMeshRenderRequest;
class Material;

class GLStaticMeshComponent
{
public:
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    GLStaticMeshComponent(GLSceneRenderer* renderer, 
                            ResourcePath staticMeshPath, 
                            const std::vector<std::shared_ptr<Material>>& newMaterials,
                            glm::vec2 lightMapUVTiling,
                            glm::vec2 lightMapUVOffset,
                            ResourcePath lightMapTexPath);

    ~GLStaticMeshComponent(){}
    void updateRequests();
    void addRequestsToRenderList();

private:
    void setup();
    
    GLSceneRenderer* _renderer = nullptr;
    ResourcePath _staticMeshPath = "";
    std::vector<std::shared_ptr<Material>> _materials;
    ResourcePath _lightMapTexPath = "";
    glm::vec2 _lightMapUVTiling = glm::vec2(0.0);
    glm::vec2 _lightMapUVOffset = glm::vec2(0.0);
    std::vector<std::shared_ptr<GLStaticMeshRenderRequest>> _renderRequests;
};

} // namespace MSGE
