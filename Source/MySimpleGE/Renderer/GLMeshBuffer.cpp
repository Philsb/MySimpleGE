#include <MySimpleGE/Renderer/GLMeshBuffer.h>
#include <MySimpleGE/Core/Mesh.h>

namespace MSGE 
{
    GLMeshBuffer::GLMeshBuffer():
    _status(GLResStatus::NOT_INITIALIZED)
    {
    }

    GLMeshBuffer::~GLMeshBuffer()
    {
        glDeleteVertexArrays(1, &_vao);
        glDeleteBuffers(1, &_vbo);
    }

    void GLMeshBuffer::createBuffers(const std::vector<MeshVertex>& vertices ) 
    {
        glGenVertexArrays(1, &_vao);  
        glGenBuffers(1, &_vbo); 

        glBindVertexArray(_vao);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo); 

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(MeshVertex), &vertices.front(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        
        glEnableVertexAttribArray(0);

        //After setup the buffers are unbound
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        _status = GLResStatus::VALID;
    }

    void GLMeshBuffer::bind()
    {
        glBindVertexArray(_vao);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo); 
    }

    void GLMeshBuffer::unbind()
    {
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
}
