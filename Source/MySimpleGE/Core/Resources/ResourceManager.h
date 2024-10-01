#pragma once
#include <MySimpleGE/Core/ReferenceManager.h>
#include <MySimpleGE/Core/Resources/Resource.h>
#include <type_traits>
#include <string>

namespace MSGE 
{
class ResourceManager: public ReferenceManager<IResource,std::string>
{
public:
    ResourceManager();
    ~ResourceManager();

    template<typename T>
    std::shared_ptr<T> load(const std::string& path)
    {
        static_assert(std::is_base_of<IResource, T>::value, "loadable class is not of type Resource\n");
        std::shared_ptr<T> resourceRef = std::dynamic_pointer_cast<T>(getRef(path));
        if (!resourceRef)
        {
            std::cout << "Not found resource in cache. creating: " << path << std::endl;
            resourceRef = std::dynamic_pointer_cast<T>( addRefUnique(path, static_cast<IResource*>(new T()) ) );
        }
        assert(resourceRef);
        return resourceRef;
        
        /*
        std::shared_ptr<T> resourceRef;
        auto refIterator = _cachedResources.find(path);

        // If the reference isn't found then instanciate a resource of type T and add it to the _cachedResources
        if (refIterator == _cachedResources.end())
        {
            auto deleter = [=](IResource* resPtr) 
            { 
                this->_cachedResources.erase(path); 
                delete resPtr;
            };
            resourceRef = std::shared_ptr<T>(new T(), deleter);
            _cachedResources.insert({path, resourceRef});
        }
        else 
        {
            resourceRef = std::dynamic_pointer_cast<T>(refIterator->second.lock());
        }
       

        assert(resourceRef);
        return resourceRef;
         */
    }
};
}

