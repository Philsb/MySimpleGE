#pragma once
#include <glad/glad.h>
#include <MySimpleGE/Core/ReferenceManager.h>
#include <MySimpleGE/Renderer/GLResource.h>
#include <list>
#include <string>
#include <memory>
#include <cassert>

namespace MSGE 
{

class GLStaticModelRenderRequest;

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

    void addToRenderList(std::shared_ptr<GLStaticModelRenderRequest> renderRequest);

private:
    std::list<std::shared_ptr<GLStaticModelRenderRequest>> _renderList;
    ReferenceManager<IGLResource, std::string> _glResourceManager;

};
}