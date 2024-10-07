#pragma once 
#include <MySimpleGE/Core/Resources/Resource.h>
#include <vector>

namespace MSGE
{
class Texture2dResource : public IResource
{
public:
    Texture2dResource();
    ~Texture2dResource();

    void load(const ResourcePath& resPath) override;
    inline int getWidth() const {return _width;};
    inline int getHeight() const {return _height;};
    const std::vector<unsigned char>& getImageData() const {return _imageData;};

private:
    int _width, _height, _bitsPerChannel;
    std::vector<unsigned char> _imageData;
};
}