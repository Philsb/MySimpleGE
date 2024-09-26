#include <MySimpleGE/Renderer/OpenGLRenderer.h>
#include <MySimpleGE/Core/Mesh.h>
namespace MSGE 
{
OpenGLRenderer::OpenGLRenderer() 
{

}

OpenGLRenderer::~OpenGLRenderer()
{
}

int OpenGLRenderer::init(GLADloadproc glLoader) 
{
    if (!gladLoadGLLoader(glLoader))
        return 1;

    const std::string vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
    const std::string fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

    _shader.createShader(vertexShaderSource, fragmentShaderSource);

    if (_shader.getStatus() != GLSLShaderStatus::SHADER_VALID)
    {
        return 1;
    }
    
    _shader.useShader();

    //Buffer creation;
    Mesh mesh1(0.0);
    _meshBuffer1.createBuffers(mesh1.getVertices());

    //Buffer creation;
    Mesh mesh2(-.5);
    _meshBuffer2.createBuffers(mesh2.getVertices());

    return 0;
}

void OpenGLRenderer::render() 
{
    _meshBuffer1.bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);

    _meshBuffer2.bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);

}

}
