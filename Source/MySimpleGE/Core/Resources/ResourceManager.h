#pragma once
#include <MySimpleGE/Core/ReferenceManager.h>
#include <MySimpleGE/Core/Resources/Resource.h>
#include <type_traits>
#include <string>
#include <filesystem>
#include <cassert>
#include <MySimpleGE/Core/Utils/Timer.h>

namespace MSGE 
{
class ResourceManager: public ReferenceManager<IResource,std::string>
{
public:
    ResourceManager();
    ~ResourceManager();

    template<typename T>
    std::shared_ptr<T> load(const ResourcePath& resPath)
    {
        static_assert(std::is_base_of<IResource, T>::value, "Loaded class is not of type Resource\n");

        if (!std::filesystem::exists( resPath.getPath() ))
        {
            std::cerr << "Error: provided file doesn't exist: " << resPath.getPath() << "\n";
            return std::shared_ptr<T>();
        }

        std::shared_ptr<T> resourceRef = std::dynamic_pointer_cast<T>(getRef(resPath.getRelativePath()));
        if (!resourceRef)
        {
            //std::cout << "Not found resource in cache. creating: " << resPath.getRelativePath() << std::endl;
            resourceRef = std::dynamic_pointer_cast<T>( addRefUnique(resPath.getRelativePath(), static_cast<IResource*>(new T()) ) );
            resourceRef->load(resPath);
        }
        assert(resourceRef);
        return resourceRef;
    }
};
}

