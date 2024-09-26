#include <MySimpleGE/Core/Mesh.h>

namespace MSGE
{
Mesh::Mesh(float offset)
{
   _vertices = {
                {glm::vec3{-0.5f + offset, -0.5f + offset, 0.0f}},
                {glm::vec3{0.5f + offset, -0.5f + offset, 0.0f}},
                {glm::vec3{0.0f + offset,  0.5f + offset, 0.0f}}
                };
    

}
Mesh::~Mesh(){}

}