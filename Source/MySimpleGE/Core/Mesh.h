#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>

namespace MSGE 
{
struct MeshVertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv1;
};

class Mesh
{
public:
    Mesh();
    ~Mesh();
    void setGeometry(const std::vector<MeshVertex>& vertices, const std::vector<unsigned int>& indices);
    const std::vector<MeshVertex>& getVertices() const {return _vertices;}
    const std::vector<unsigned int>& getIndices() const {return _indices;}
private:
    std::vector<MeshVertex> _vertices;
    std::vector<unsigned int> _indices;
};

}