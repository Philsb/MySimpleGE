#include <MySimpleGE/Renderer/GLFrameBuffer.h>

namespace MSGE
{

GLFrameBuffer::GLFrameBuffer()
{
    create();
}
GLFrameBuffer::~GLFrameBuffer()
{
    glDeleteFramebuffers(1, &_fbo);  
}
void GLFrameBuffer::create()
{
    glGenFramebuffers(1, &_fbo);
}
void GLFrameBuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);  
}
void GLFrameBuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);  
}
    
} // namespace MSGE
