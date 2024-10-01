#include <MySimpleGE/Renderer/GLTexture2d.h>
#include <iostream>

namespace MSGE
{
    
GLTexture2d::GLTexture2d()
{
    create();
}

GLTexture2d::~GLTexture2d()
{
    glDeleteTextures(1, &_textureId);
}

void GLTexture2d::create()
{
    glGenTextures(1, &_textureId);  
}

void GLTexture2d::setData(int width, int height, const std::vector<unsigned char>& imageData)
{
    bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &imageData.front());
    //glGenerateMipmap(GL_TEXTURE_2D);
    unbind();
}

void GLTexture2d::bind()
{
    glBindTexture(GL_TEXTURE_2D, _textureId);
}
void GLTexture2d::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

} // namespace MSGE
