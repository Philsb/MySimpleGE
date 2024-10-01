#include <MySimpleGE/Core/Resources/MeshResource.h>
#include <nlohmann/json.hpp>
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
void MeshResource::load(const std::string& path)
{
    std::ifstream f(path);

    if (!f)
    {
        std::cerr << "WARNING: Couldn't open file: " << path << "\n";
        return;
    }


    json data = json::parse(f);
    auto assetTypeIt = data.find("res_type"); 

    if (assetTypeIt == data.end()) {
        std::cerr << "ERROR Loaded resource from: " << path << ", doesn't have asset_type key" "\n";
        return;
    }

    if (*assetTypeIt != "mesh")
    {
        std::cerr << "ERROR Loaded resource from: " << path << ", is not of MESH type" "\n";
        return;
    }


    bool errorReading = false;
    auto verticesObjIt = data.find("vertices");
    std::vector<MeshVertex> vertices;
    vertices.reserve(4);
    if (verticesObjIt != data.end())
    {
        auto verticesArray = *verticesObjIt;
        if (verticesArray.is_array())
        {
            auto verticesIt = verticesArray.begin();
            while (verticesIt != verticesArray.end())
            {

                MeshVertex v;
                double readingBuf[8] = {0};
                for (int i = 0; i < 8; i++)
                {
                    if (verticesIt == verticesArray.end())
                        break;
                    if ((*verticesIt).is_number())
                        readingBuf[i] = *verticesIt;
                    ++verticesIt;
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

                vertices.push_back(v);
            }
        }
        else 
        {
            return;
        }
    }
    else 
    {
        return;
    }

    auto indicesObjIt = data.find("indices");
    std::vector<unsigned int> indices;
    indices.reserve(4);
    if (indicesObjIt != data.end())
    {
        auto indicesArray = *indicesObjIt;
        if (indicesArray.is_array())
        {
            for (auto v : indicesArray)
            {
                if (v.is_number())  
                    indices.push_back(v);
            }
        }
        else 
        {
            return;
        }
    }

    _meshData.setGeometry(vertices, indices);

    std::cout << "Loaded MESH resource from: " << path << "\n";
}

}