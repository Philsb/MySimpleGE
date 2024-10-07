#include <MySimpleGE/Core/Resources/Material.h>
#include <MySimpleGE/Core/Utils/JsonUtils.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <regex>
#include <iostream>

using json = nlohmann::json;

namespace MSGE
{

Material::Material()
: _shaderPath("")
{
}

Material::~Material()
{
}


void Material::load(const ResourcePath& resPath)
{  
    std::ifstream f(resPath.getPath());
    if (!f)
    {
        std::cerr << "ERROR: Couldn't open file: " << resPath.getPath() << "\n";
        return;
    }

    json data = json::parse(f);
    if (JsonUtils::hasValidResType(data, "material"))
    {
        std::cerr << "ERROR: Loaded resource from: " << resPath.getPath() << ", is not of MATERIAL type or does not have resType key\n";
        return;
    }

    auto shaderPathValue = data["shaderPath"];
    if (shaderPathValue.is_string())
    {
        _shaderPath = shaderPathValue.get<std::string>();     
    }

    auto textParamsArrayValue = data["textureParams"];
    if (textParamsArrayValue.is_array())
    {
        auto textParamsArray = textParamsArrayValue.get<json::array_t>();
        _textureParams.clear();
        for (auto texParams : textParamsArray)
        {
            
            if (texParams.is_object())
            {
                std::string paramName;
                std::string texturePath;
                auto paramNameValue = texParams["paramName"];
                if (paramNameValue.is_string())
                {
                    paramName = paramNameValue.get<std::string>();
                }

                auto texPathValue = texParams["texturePath"];
                if (texPathValue.is_string())
                {
                    texturePath =  texPathValue.get<std::string>();
                }

                setTextureParam(paramName,texturePath);
            }
        }
    }
}

void Material::setShaderPath(const ResourcePath& shaderPath)
{
    _shaderPath = shaderPath;
}

ShaderSource Material::getShaderSource() const
{
    //TODO: loading shader source is taking too much, maybe use pure C functions?
    //Maybe is the regex?
    std::ifstream file(_shaderPath);

    ShaderSource ss;
    enum class ShaderReadingState
    {
        NONE = -1, VERT = 0, FRAG = 1
    };

    ShaderReadingState readState = ShaderReadingState::NONE;
    if (file)
    {   
        std::string line;
        while(getline(file, line))
        {
            if (std::regex_match(line, std::regex("\\s*#vertShader\\s*")))
            {
                readState = ShaderReadingState::VERT;
            }
            else if (std::regex_match(line, std::regex("\\s*#fragShader\\s*")))  
            {
                readState = ShaderReadingState::FRAG;
            }
            else if (readState != ShaderReadingState::NONE)
            {
                if (readState == ShaderReadingState::VERT)
                    ss.vShaderSource += line + "\n";
                else if (readState == ShaderReadingState::FRAG)
                    ss.fShaderSource += line + "\n";
            }
        }

    }
    else
    {
        std::cerr << "Could not find shader with file: " << _shaderPath.getPath() << "\n";
    }
    return ss;
 }

void Material::setTextureParam(const std::string& paramName, std::string texturePath)
{
    if (_textureParams.size() < MAX_TEXTURES)
    {
        _textureParams.push_back({paramName, texturePath});
    }
    else
    {
        std::cerr << "Material is already at max slots\n";
    }
}
void Material::setFloatParam(const std::string& paramName, float value)
{
    _vecParams.insert_or_assign(paramName, value);
}
void Material::setVec2Param(const std::string& paramName, glm::vec2 value)
{
    _vecParams.insert_or_assign(paramName, value);
}
void Material::setVec3Param(const std::string& paramName, glm::vec3 value)
{
    _vecParams.insert_or_assign(paramName, value);
}
void Material::setVec4Param(const std::string& paramName, glm::vec4 value)
{
    _vecParams.insert_or_assign(paramName, value);
}
void Material::setMat2Param(const std::string& paramName, glm::mat2 value)
{
    _matrixParams.insert_or_assign(paramName, value);
}
void Material::setMat3Param(const std::string& paramName, glm::mat3 value)
{
    _matrixParams.insert_or_assign(paramName, value);
}
void Material::setMat4Param(const std::string& paramName, glm::mat4 value)
{
    _matrixParams.insert_or_assign(paramName, value);
}
} // namespace MSGE

 