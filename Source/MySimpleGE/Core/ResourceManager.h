#pragma once
#include <MySimpleGE/Core/Resource.h>
#include <map>
#include <string>

namespace MSGE 
{
class ResourceManager
{
public:
    ResourceManager()
    {
        std::cout<<"Res manager created" << std::endl;
    };

    ~ResourceManager()
    {
        std::cout<<"Res manager destroyed" << std::endl;
    }
private:
    std::map<std::string, Resource> _cachedResources;
};
}