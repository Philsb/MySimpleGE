#pragma once
#include <MySimpleGE/Core/Resources/Resource.h>
#include<string>
#include<map>
#include<variant>
#include<vector>
#include<glm/glm.hpp>

namespace MSGE
{
using VecParamVariant = std::variant <float, glm::vec2, glm::vec3, glm::vec4>;
using MatrixParamVariant = std::variant <glm::mat2, glm::mat3, glm::mat4>;

struct ShaderSource
{
    std::string vShaderSource;
    std::string fShaderSource;
};

struct TextureParam
{
    std::string paramName;
    ResourcePath texturePath;
};

class Material : public IResource
{
public:
    static constexpr unsigned int MAX_TEXTURES = 16;

    Material();
    ~Material();

    void load(const ResourcePath& resPath) override;
    ShaderSource getShaderSource() const;
    void setShaderPath(const ResourcePath& shaderPath);
    
    inline const ResourcePath getShaderPath() const {return _shaderPath;}
    inline const std::vector<TextureParam>& getTextureParams() const {return _textureParams;}

    void setTextureParam(const std::string& paramName, std::string texturePath);
    void setFloatParam(const std::string& paramName, float value);
    void setVec2Param(const std::string& paramName, glm::vec2 value);
    void setVec3Param(const std::string& paramName, glm::vec3 value);
    void setVec4Param(const std::string& paramName, glm::vec4 value);
    void setMat2Param(const std::string& paramName, glm::mat2 value);
    void setMat3Param(const std::string& paramName, glm::mat3 value);
    void setMat4Param(const std::string& paramName, glm::mat4 value);

private:
    ResourcePath _shaderPath;
    //TODO: what about double precision floats?
    //TODO: maybe we want bool, and integer versions?
    //TODO: Uniform buffers?
    std::map<std::string, VecParamVariant> _vecParams;
    std::map<std::string, MatrixParamVariant> _matrixParams;
    //TODO: do we want max texture amount to be 16??
    std::vector<TextureParam> _textureParams;
};

} // namespace MSGE
