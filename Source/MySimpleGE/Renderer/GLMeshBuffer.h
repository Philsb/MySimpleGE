#pragma once
#include <glad/glad.h>
#include <MySimpleGE/Renderer/GLCommonInclude.h>
#include <MySimpleGE/Core/Mesh.h>
#include <vector>

namespace MSGE
{

class GLMeshBuffer
{
public:
    GLMeshBuffer();
    ~GLMeshBuffer();
    void createBuffers(const std::vector<MeshVertex>& vertices);
    GLResStatus getStatus(){return _status;}
    void bind();
    void unbind();

private:
    GLuint _vao, _vbo, _ibo;
    GLResStatus _status;
};
}
