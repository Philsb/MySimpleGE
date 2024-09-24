#pragma once
#include <glad/glad.h>
#include <MySimpleGE/Renderer/GLCommonInclude.h>
#include <vector>

namespace MSGE
{

class GLMeshBuffer
{
    GLMeshBuffer();
    void createBuffers();
    GLResStatus getStatus(){return _status;};
    ~GLMeshBuffer();

private:
    GLuint _vao, _vbo, _ibo;
    GLResStatus _status;

    std::vector<> vertexData;
    std::vector<> indexData;

};
}
