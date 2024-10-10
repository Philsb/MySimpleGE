#include <MySimpleGE/Renderer/GLMeshBuffer.h>
#include <MySimpleGE/Core/Mesh.h>

namespace MSGE 
{
    GLMeshBuffer::GLMeshBuffer()
    :IGLResource(),
    _vao(-1),
    _vbo(-1),
    _ibo(-1),
    _indexCount(0),
    _status(GLResStatus::NOT_INITIALIZED)
    {
        create();
    }

    GLMeshBuffer::~GLMeshBuffer()
    {
        glDeleteVertexArrays(1, &_vao);
        glDeleteBuffers(1, &_vbo);
        glDeleteBuffers(1, &_ibo);
    }
    
    void GLMeshBuffer::create()
    {
        glGenVertexArrays(1, &_vao);  
        glGenBuffers(1, &_vbo); 
        glGenBuffers(1, &_ibo);

        glBindVertexArray(_vao);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo); 
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo); 

        //glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(MeshVertex), &vertices.front(), GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // Normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(3* sizeof(float)));
        glEnableVertexAttribArray(1);
        // uv1 attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(6* sizeof(float)));
        glEnableVertexAttribArray(2);

        // lightmap attribute
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(8* sizeof(float)));
        glEnableVertexAttribArray(3);

        _status = GLResStatus::VALID;

        unbind();
    }

    void GLMeshBuffer::setData(const std::vector<MeshVertex>& vertices, const std::vector<unsigned int>& indices) 
    {
        bind();

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices.front(), GL_STATIC_DRAW);
        
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(MeshVertex), &vertices.front(), GL_STATIC_DRAW);
        
        _indexCount = indices.size();

        unbind();
        _status = GLResStatus::VALID;
    }
    

    void GLMeshBuffer::bind()
    {
        glBindVertexArray(_vao);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo); 
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo); 
    }

    void GLMeshBuffer::unbind()
    {
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 
    }
    
}
