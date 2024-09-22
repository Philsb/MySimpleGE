#pragma once
#include <glad/glad.h>
#include <MySimpleGE/Renderer/GLSLShader.h>

namespace MSGE 
{

class OpenGLRenderer {
public:
    OpenGLRenderer();
    ~OpenGLRenderer();

    int init(GLADloadproc);
    void render();
private:
    GLuint _triangleVBO;
    GLuint _vao;
    GLSLShader _shader;

};
}