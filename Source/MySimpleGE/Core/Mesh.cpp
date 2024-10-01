#include <MySimpleGE/Core/Mesh.h>

namespace MSGE
{
Mesh::Mesh()
{  

}
Mesh::~Mesh()
{
}

void Mesh::setGeometry(const std::vector<MeshVertex>& vertices, const std::vector<unsigned int>& indices)
{
   _vertices = std::move(vertices);
   _indices = std::move(indices);
}

}