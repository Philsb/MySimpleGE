#pragma once
#include <string>

namespace MSGE 
{
class IResource
{
public:
    IResource(){}
    virtual ~IResource(){}

    virtual void load(const std::string& path) = 0;
    //virtual void save() = 0;
private:
    //ResourceStatus _resStatus; 
};

}