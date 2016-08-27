
/**
*
* Copyright (c) 2014 : William Taylor : wi11berto@yahoo.co.uk
*
* This software is provided 'as-is', without any
* express or implied warranty. In no event will
* the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute
* it freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented;
*    you must not claim that you wrote the original software.
*    If you use this software in a product, an acknowledgment
*    in the product documentation would be appreciated but
*    is not required.
*
* 2. Altered source versions must be plainly marked as such,
*    and must not be misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any source distribution.
*/

#include "ModelFile.h"

// Constructor & Deconstructor
ModelAsset::ModelAsset(std::string name)
{
    this->name = name;
}

ModelAsset::~ModelAsset()
{
    // cleanup all textures loaded
    for (auto& t : textures) 
        SAFE_RELEASE(t);
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

// grabs the model from disk and loads it into memory via ASSIMP
void ModelAsset::grabFromFile(const char * filename)
{
    Assimp::Importer Importer;

    // create a pointer to the loaded structure
    const aiScene * scene = Importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_GenSmoothNormals);

    // if model failed to load
    if (scene == NULL)
    {
        // throw an exception
        throw Error(Component::System, std::string("couldnt load model ", filename));
    }
    else
    {
        // else populate vectors with information about the model (vertices, normals etc)
        setupFromScene(scene, filename);
    }
}

void ModelAsset::recycle()
{

}

// goes through each method and calls the setupMesh and setupMaterials private functions
bool ModelAsset::setupFromScene(const aiScene * scene, const std::string& filename)
{
    // identify the number of meshes
    textures.resize(scene->mNumMaterials);
    meshes.resize(scene->mNumMeshes);

    for (unsigned int i = 0; i < meshes.size(); i++)
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
    std::string::size_type SlashIndex = filename.find_last_of("/");
    std::string Dir = "";

    if (SlashIndex != 0 && SlashIndex != std::string::npos) {
        Dir = filename.substr(0, SlashIndex + 1);
    }

    
    for (unsigned int i = 0; i < pScene->mNumMaterials; i++) {
        const aiMaterial* pMaterial = pScene->mMaterials[i];
        textures[i] = NULL;

        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString Path;
            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
                std::string nm = Path.C_Str();
                std::string FullPath = Dir + Path.data;
                //std::cout << Dir + Path.data << std::endl;
                
                if (nm.find("HANG") != std::string::npos)
                {
                    textures[i] = new TextureAsset(FullPath.substr(0, FullPath.length() - 4));
                    textures[i]->grabFromFile("data/models/hang/hang2.bmp");
                }
                else
                {				
                    textures[i] = new TextureAsset(FullPath.substr(0, FullPath.length() - 4));
                    textures[i]->grabFromFile(FullPath.c_str());
                }
            }
        }
    }

    return true;
}

std::string ModelAsset::getName()
{
    return name;
}

vector<TextureAsset *>& ModelAsset::getTextures()
{
    return textures;
}

vector<ModelAsset::MeshEntry *>& ModelAsset::getMeshes()
{
    return meshes;
}

// sets up each individual mesh and pushes mesh info into the internal vectors
bool ModelAsset::setupMesh(unsigned int ID, const aiMesh * mesh)
{
    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    // then go through the details of the mesh inserting useful data into to the vectors
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) 
    {
        // here we just get the normals vertices and texture coordinates
        const aiVector3D * u = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][i]) : &Zero3D;
        const aiVector3D * n = mesh->HasNormals() ? &(mesh->mNormals[i]) : &Zero3D;
        const aiVector3D * v = &(mesh->mVertices[i]);

        // push back the data into the vectors
        vertices.push_back(vec3(v->x, v->y, v->z));
        normals.push_back(vec3(n->x, n->y, n->z));
        uvs.push_back(vec3(u->x, u->y, u->z));
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        const aiFace& Face = mesh->mFaces[i];
        
        assert(Face.mNumIndices == 3);
        
        indices.push_back(Face.mIndices[0]);
        indices.push_back(Face.mIndices[1]);
        indices.push_back(Face.mIndices[2]);
    }

    return true;
}