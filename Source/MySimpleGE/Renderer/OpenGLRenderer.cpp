#include <MySimpleGE/Renderer/OpenGLRenderer.h>

namespace MSGE 
{
OpenGLRenderer::OpenGLRenderer() 
{

}

int OpenGLRenderer::init(GLADloadproc glLoader) 
{
    if (!gladLoadGLLoader(glLoader))
        return 1;

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

    glGenVertexArrays(1, &_vao);  
    glBindVertexArray(_vao);

    //----------

    glGenBuffers(1, &_triangleVBO); 

    glBindBuffer(GL_ARRAY_BUFFER, _triangleVBO); 

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    
    glEnableVertexAttribArray(0);  


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

    return 0;
}

void OpenGLRenderer::render() 
{
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

OpenGLRenderer::~OpenGLRenderer()
{
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_triangleVBO);
}

}
