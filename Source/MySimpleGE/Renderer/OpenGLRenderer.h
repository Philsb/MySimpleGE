#pragma once
#include <glad/glad.h>
#include <MySimpleGE/Renderer/GLSLShader.h>
#include <MySimpleGE/Renderer/GLMeshBuffer.h>

namespace MSGE 
{

class OpenGLRenderer {
public:
    OpenGLRenderer();
    ~OpenGLRenderer();

    int init(GLADloadproc);
    void render();
private:
    GLMeshBuffer _meshBuffer1;
    GLMeshBuffer _meshBuffer2;
    GLSLShader _shader;

};
}