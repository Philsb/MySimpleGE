#include <MySimpleGE/Core/Resources/MeshResource.h>
#include <nlohmann/json.hpp>
#include <MySimpleGe/Core/Utils/JsonUtils.h>
#include <MySimpleGE/Core/Utils/Timer.h>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

namespace MSGE
{
MeshResource::MeshResource()
{
}

MeshResource::~MeshResource()
{
}

void MeshResource::load(const ResourcePath& resPath)
{
    std::ifstream f(resPath.getPath());
    if (!f)
    {
        std::cerr << "WARNING: Couldn't open file: " << resPath.getPath() << "\n";
        return;
    }

    //TODO: VALIDATE with non hardcoded res types, like const resMeshType = "mesh" or something like that  
    const json& data = json::parse(f);
    if (JsonUtils::hasValidResType(data, "mesh"))
    {
        std::cerr << "ERROR Loaded resource from: " << resPath.getPath() << ", is not of MESH type or does not have resType key\n";
        return;
    }
    
    bool errorReading = false;
    std::vector<MeshSegment> meshSegments;

    const auto& meshesArrayValue = data["meshes"];

    if (meshesArrayValue.is_array())
    {
        const auto& meshesArray = meshesArrayValue.get<json::array_t>();   
        for (const auto& mesh : meshesArray)
        {
            //TODO: reserve N amount of vertices from the start using the json array amount
            std::vector<MeshVertex> currentVerVector;
            std::vector<unsigned int> currentIdxVector;
            

            const auto& verticesArrayValue = mesh["vertices"];
            if (verticesArrayValue.is_string())
            {
                const std::string& verticesArray = verticesArrayValue;
                const std::vector<unsigned char>& base64Buffer = JsonUtils::base64_decode(verticesArray);
                // Convert byte array to float array
                size_t numVerts = base64Buffer.size() / sizeof(MeshVertex);
                //std::vector<float> floatArray(numFloats);
                currentVerVector.resize(numVerts);
                std::memcpy(currentVerVector.data(), base64Buffer.data(), base64Buffer.size());
                

                //currentVerVector.reserve(verticesArray.size());
                /*
                auto verticesArrayIt = verticesArray.begin();
                while (verticesArrayIt != verticesArray.end())
                {
                    MeshVertex v;
                    double readingBuf[8] = {0};
                    for (int i = 0; i < 8; i++)
                    {
                        if (verticesArrayIt == verticesArray.end())
                            break;
                        if ((*verticesArrayIt).is_number())
                            readingBuf[i] = *verticesArrayIt;
                        ++verticesArrayIt;
                    }

                    //TODO: Is there a better way to do this?
                    v.position.x = readingBuf[0];
                    v.position.y = readingBuf[1];
                    v.position.z = readingBuf[2];
                    v.normal.x = readingBuf[3];
                    v.normal.y = readingBuf[4];
                    v.normal.z = readingBuf[5];
                    v.uv1.x = readingBuf[6];
                    v.uv1.y = readingBuf[7];

                    currentVerVector.push_back(v);
                }
                */
            }
            
            const auto& indicesArrayValue = mesh["indices"];
            if (indicesArrayValue.is_string())
            {

                const std::string& indicesArray = indicesArrayValue;
                const std::vector<unsigned char>& base64Buffer = JsonUtils::base64_decode(indicesArray);
                
                // Convert byte array to float array
                size_t numIdxs = base64Buffer.size() / sizeof(unsigned int);
                //std::vector<float> floatArray(numFloats);
                currentIdxVector.resize(numIdxs);
                std::memcpy(currentIdxVector.data(), base64Buffer.data(), base64Buffer.size());

                
                //const auto& indicesArray = indicesArrayValue.get<json::array_t>();

                /*
                currentIdxVector.reserve(indicesArray.size());
                for (const auto& idxValue : indicesArray)
                {
                    if (idxValue.is_number())  
                        currentIdxVector.push_back(idxValue);
                }*/

            }
              
            meshSegments.push_back({std::move(currentVerVector), std::move(currentIdxVector)});

        }
    }

    _meshData.setGeometry(std::move(meshSegments));
    //std::cout << "Loaded MESH resource from: " << resPath.getPath() << "\n";
}

}