#include <MySimpleGE/Renderer/GLMeshBuffer.h>

namespace MSGE 
{
    GLMeshBuffer::GLMeshBuffer():
    _status(GLResStatus::NOT_INITIALIZED)
    {
    }

    void GLMeshBuffer::createBuffers(const std::vector<>& ) 
    {
        glGenVertexArrays(1, &_vao);  
        
        glBindVertexArray(_vao);

        glGenBuffers(1, &_triangleVBO); 

        glBindBuffer(GL_ARRAY_BUFFER, _triangleVBO); 

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        
        glEnableVertexAttribArray(0);
    }


    GLMeshBuffer::~GLMeshBuffer()
    {

    }
    

}
