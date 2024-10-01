#pragma once
#include <glad/glad.h>
#include <MySimpleGE/Renderer/GLCommonInclude.h>
#include <MySimpleGE/Renderer/GLResource.h>
#include <vector>

namespace MSGE
{


struct MeshVertex;
class GLMeshBuffer : public IGLResource
{
public:
    GLMeshBuffer();
    ~GLMeshBuffer();

    void create() override;
    void bind() override;
    void unbind() override;
    unsigned int getIndexCount() const {return _indexCount;}
    void setData(const std::vector<MeshVertex>& vertices, const std::vector<unsigned int>& indices);
    GLResStatus getStatus(){return _status;}

private:
    GLuint _vao, _vbo, _ibo;
    GLuint _indexCount;
    GLResStatus _status;

    
};
}
