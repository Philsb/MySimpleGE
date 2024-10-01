#pragma once
#include <MySimpleGE/Renderer/GLResource.h>
#include <glad/glad.h>
#include <vector>

namespace MSGE
{
    
class GLTexture2d : public IGLResource
{
public:
    GLTexture2d();
    ~GLTexture2d();
    
    void create() override;
    void bind()override;
    void unbind()override;

    void setData(int width, int height, const std::vector<unsigned char>& imageData);
private:
    GLuint _textureId;
};

} // namespace MSGE
