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

struct MeshSegment
{
    std::vector<MeshVertex> vertices;
    std::vector<unsigned int> indices;
};

/*There are a lot of ways to describe a mesh and THIS is subject to CHANGE, other alternatives are:

    1. Higher level mesh description with faces, edges... and a mutable cache to store the buffers
    2. Only the higher level mesh without the cache and it generates the rendering buffer on demand.
    3. packed rendering buffer with offsets indicating the different segments...

*/

class Mesh
{
public:
    Mesh();
    ~Mesh();
    
    void setGeometry(const std::vector<MeshSegment>& meshSegments);
    void setGeometry(std::vector<MeshSegment>&& meshSegments);

    const std::vector<MeshSegment>& getSegments() const {return _segments;}
    inline int getSegmentAmount() const {return _segments.size();}
private:
    //Each vector of vector represents the mesh segments
    std::vector<MeshSegment> _segments;
};

}