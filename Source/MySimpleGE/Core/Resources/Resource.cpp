#include <MySimpleGE/Core/Resources/Resource.h>

namespace MSGE
{

ResourcePath::ResourcePath(){}
ResourcePath::ResourcePath(const std::string& path) : _relativePath(path){}

ResourcePath& ResourcePath::operator=(const std::string& path)
{
    _relativePath = path;
    return *this;
}

ResourcePath& ResourcePath::operator=(const ResourcePath& other)
{
    if (this == &other)
            return *this;
    _relativePath = other._relativePath;
    return *this;
}

} // namespace MSGE
