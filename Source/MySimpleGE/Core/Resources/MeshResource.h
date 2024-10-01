#pragma once 
#include <MySimpleGE/Core/Resources/Resource.h>
#include <MySimpleGE/Core/Mesh.h>

namespace MSGE
{

class MeshResource: public IResource
{
public:
    MeshResource();
    ~MeshResource();
    void load(const std::string& path) override;
    const Mesh& getMeshData() const {return _meshData;}
private:
    Mesh _meshData;

};


}