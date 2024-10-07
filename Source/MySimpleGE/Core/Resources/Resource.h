#pragma once
#include <string>

namespace MSGE 
{

class ResourcePath
{
public:
    ResourcePath();
    ResourcePath(const std::string& path);
    
    //constructor for pure string literals
    template<std::size_t N> 
    ResourcePath(const char (&charString)[N])
    : _relativePath(charString){};

    ResourcePath& operator=(const std::string& path);
    ResourcePath& operator=(const ResourcePath& other);

    inline std::string getRelativePath() const {return _relativePath;}
    inline std::string getPath() const {return RESOURCES_PATH + _relativePath;}
    inline operator std::string() const {return getPath();}

private:
    std::string _relativePath;
};

class IResource
{
public:
    IResource(){}
    virtual ~IResource(){}

    virtual void load(const ResourcePath& resPath) = 0;
    //virtual void save() = 0;
private:
    //ResourceStatus _resStatus; 
};

}