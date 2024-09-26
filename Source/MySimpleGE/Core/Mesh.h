#pragma once
#include <MySimpleGE/Core/Resource.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

namespace MSGE 
{
struct MeshVertex
{
    glm::vec3 position;
};

class Mesh: public Resource
{
public:
    Mesh(float offset);
    ~Mesh();
    const std::vector<MeshVertex>& getVertices() const {return _vertices;}

private:
    std::vector<MeshVertex> _vertices;

};

}