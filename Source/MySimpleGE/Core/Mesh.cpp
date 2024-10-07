#include <MySimpleGE/Core/Mesh.h>
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL 1
#include <glm/gtx/string_cast.hpp>
#include <string>


namespace MSGE
{
Mesh::Mesh()
{  

}

Mesh::~Mesh()
{
}


void Mesh::setGeometry(const std::vector<MeshSegment>& meshSegments)
{
   _segments.clear();
   _segments.reserve(meshSegments.size());

   for(auto meshSegment : meshSegments)
   {
      _segments.push_back(meshSegment);
   }
}

void Mesh::setGeometry(std::vector<MeshSegment>&& meshSegments)
{
   _segments.clear();
   _segments.reserve(meshSegments.size());

   for(auto meshSegment : meshSegments)
   {
      _segments.push_back(std::move(meshSegment));
   }

}

}