#pragma once
#include <glad/glad.h>
#include <MySimpleGE/Renderer/GLResource.h>

namespace MSGE
{
class GLFrameBuffer : public IGLResource
{
public:
    GLFrameBuffer();
    ~GLFrameBuffer();
    void create() override;
    void bind() override;
    void unbind() override;
private:
    GLuint _fbo = -1;
};
    
} // namespace MSGE