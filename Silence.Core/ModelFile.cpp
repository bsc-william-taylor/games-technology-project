
#include "ModelFile.h"

ModelAsset::ModelAsset(std::string name)
{
    this->name = name;
}

ModelAsset::~ModelAsset()
{
    for (auto& t : textures) 
    {
        SAFE_RELEASE(t);
    }
}

vector<unsigned int>& ModelAsset::getIndices()
{
    return indices;
}

vector<vec3>& ModelAsset::getVertices()
{
    return vertices;
}

vector<vec3>& ModelAsset::getNormals()
{
    return normals;
}

vector<vec3>& ModelAsset::getUvs()
{
    return uvs;
}

void ModelAsset::grabFromFile(const char * filename)
{
    Assimp::Importer Importer;

    const auto scene = Importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_GenSmoothNormals);

    if (scene == nullptr)
    {
        throw Error(Component::System, std::string("couldnt load model ", filename));
    }
    
    setupFromScene(scene, filename);
}

bool ModelAsset::setupFromScene(const aiScene * scene, const std::string& filename)
{
    textures.resize(scene->mNumMaterials);
    meshes.resize(scene->mNumMeshes);

    for (auto i = 0u; i < meshes.size(); i++)
    {
        meshes[i] = new MeshEntry();
        meshes[i]->MaterialIndex = scene->mMeshes[i]->mMaterialIndex;
        meshes[i]->NumIndices = scene->mMeshes[i]->mNumFaces * 3;
        meshes[i]->IndexStart = (void*)(sizeof(unsigned)*indices.size());
        meshes[i]->VertexStart = vertices.size();

        setupMesh(i, scene->mMeshes[i]);
    }

    return setupMaterials(scene, filename);
}

bool ModelAsset::setupMaterials(const aiScene* pScene, const std::string& filename)
{
    auto slashIndex = filename.find_last_of("/");
    auto directory = std::string("");

    if (slashIndex != 0 && slashIndex != std::string::npos) 
    {
        directory = filename.substr(0, slashIndex + 1);
    }

    for (auto i = 0u; i < pScene->mNumMaterials; i++) 
    {
        const auto material = pScene->mMaterials[i];

        textures[i] = nullptr;

        if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) 
        {
            aiString path;

            if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path, nullptr, nullptr, nullptr, nullptr, nullptr) == AI_SUCCESS)
            {
                const auto name = std::string(path.C_Str());
                const auto fullPath = directory + path.data;
      
                // Temp fix for strange texture name
                if (name.find("HANG") != std::string::npos)
                {
                    textures[i] = new TextureAsset(fullPath.substr(0, fullPath.length() - 4));
                    textures[i]->grabFromFile("data/models/hang/hang2.bmp");
                }
                else
                {				
                    textures[i] = new TextureAsset(fullPath.substr(0, fullPath.length() - 4));
                    textures[i]->grabFromFile(fullPath.c_str());
                }
            }
        }
    }

    return true;
}

std::string ModelAsset::getName() const
{
    return name;
}

vector<TextureAsset *>& ModelAsset::getTextures()
{
    return textures;
}

vector<MeshEntry *>& ModelAsset::getMeshes()
{
    return meshes;
}

bool ModelAsset::setupMesh(unsigned int ID, const aiMesh * mesh)
{
    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) 
    {
        const auto u = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][i]) : &Zero3D;
        const auto n = mesh->HasNormals() ? &(mesh->mNormals[i]) : &Zero3D;
        const auto v = &(mesh->mVertices[i]);

        vertices.push_back(vec3(v->x, v->y, v->z));
        normals.push_back(vec3(n->x, n->y, n->z));
        uvs.push_back(vec3(u->x, u->y, u->z));
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        const auto& Face = mesh->mFaces[i];
        
        assert(Face.mNumIndices == 3);
        
        indices.push_back(Face.mIndices[0]);
        indices.push_back(Face.mIndices[1]);
        indices.push_back(Face.mIndices[2]);
    }

    return true;
}