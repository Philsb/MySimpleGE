#include<MySimpleGE/Core/Resources/Texture2dResource.h>
#include<stb_image/stb_image.h>
#include<MySimpleGE/Core/Utils/Timer.h>
#include<iostream>

namespace MSGE
{
Texture2dResource::Texture2dResource()
:_width(0),
_height(0),
_bitsPerChannel(0)
{
    _imageData.reserve(16*16);
}

Texture2dResource::~Texture2dResource()
{
    //stbi_image_free(_imageData);
}

void Texture2dResource::load(const ResourcePath& resPath)
{
    stbi_set_flip_vertically_on_load(1);
    unsigned char* loadedBuffer = stbi_load( resPath.getPath().c_str(), &_width, &_height, &_bitsPerChannel, STBI_rgb_alpha);
    if (loadedBuffer)
    {
        
        _imageData.assign(loadedBuffer , loadedBuffer+(_width*_height*4));
        /*_imageData.reserve(_width*_height*4);
        for (int i = 0; i < (_width*_height*4 ); i++)
        {
            _imageData.push_back(loadedBuffer[i]);
        }*/
        stbi_image_free(loadedBuffer);
        //std::cout << "Loaded image: " << resPath.getPath() << "\n";
        
    }
    else
    {
        std::cerr << "Couldn't load image: " << resPath.getPath() << "\n";
    } 
    stbi_set_flip_vertically_on_load(0);
}
    
} // namespace MSGE


