#pragma once
#include <glad/glad.h>
#include <MySimpleGE/Core/ReferenceManager.h>
#include <MySimpleGE/Renderer/GLResource.h>
#include <list>
#include <string>
#include <memory>
#include <cassert>
#include <glm/glm.hpp>

namespace MSGE 
{

class GLStaticMeshRenderRequest;

class OpenGLRenderer 
{
public:
    OpenGLRenderer();
    ~OpenGLRenderer();

    int init(GLADloadproc);
    void render();

    bool hasGLResource(const std::string& id);
    
    template<typename T>
    std::shared_ptr<T> getGLResource(const std::string& id)
    {
        static_assert(std::is_base_of<IGLResource, T>::value, "Requested type is not of type GLResource\n");
        auto glResourceRef = std::dynamic_pointer_cast<T>(_glResourceManager.getRef(id));
        return glResourceRef;
    }

    template<typename T>
    std::shared_ptr<T> allocateGLResource(const std::string& id)
    {
        static_assert(std::is_base_of<IGLResource, T>::value, "Requested type is not of type GLResource\n");
        auto glResourceRef = std::dynamic_pointer_cast<T>(_glResourceManager.getRef(id));
        if (!glResourceRef)
        {
            glResourceRef = std::dynamic_pointer_cast<T>( _glResourceManager.addRefUnique(id, new T() ) );
        }
        assert(glResourceRef);
        return glResourceRef;
    }

    void emptyRenderList();

    void printResources();

    void addToRenderList(std::shared_ptr<GLStaticMeshRenderRequest> renderRequest);

    glm::mat4 projectionMatrix = glm::mat4(1.0);
    glm::mat4 viewMatrix = glm::mat4(1.0);

    static const glm::mat4 ZUpMatrix;
    //static const float pi = glm::pi<float>();
    //static constexpr glm::mat4 ZUpMatrix = glm::rotate(glm::mat4(1.0), pi/2.0f, glm::vec3(1.0,0.0,0.0));

private:
    std::list<std::shared_ptr<GLStaticMeshRenderRequest>> _renderList;
    ReferenceManager<IGLResource, std::string> _glResourceManager;

};

}