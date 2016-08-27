
#pragma once

#include "EngineLayer.h"
#include "TextureAsset.h"
#include "math.h"

struct MeshEntry 
{
    unsigned int MaterialIndex;
    unsigned int NumIndices;
    unsigned int VertexStart;
    void * IndexStart;
};

using std::vector;
using glm::vec3;
using glm::vec2;

class SILENCE_EXPORT ModelAsset
{
    std::string name;

    vector<TextureAsset *> textures;
    vector<MeshEntry *> meshes;
    vector<unsigned int> indices;
    vector<vec3> vertices;
    vector<vec3> normals;
    vector<vec3> uvs;
public:
    explicit ModelAsset(std::string name);
    ~ModelAsset();

    vector<unsigned int>& getIndices();
    vector<vec3>& getVertices();
    vector<vec3>& getNormals();
    vector<vec3>& getUvs();

    vector<TextureAsset *>& getTextures();
    vector<MeshEntry *>& getMeshes();

    void grabFromFile(const char * filename);

    std::string getName() const;
private:
    bool setupMaterials(const aiScene * scene, const std::string& filename);
    bool setupFromScene(const aiScene * scene, const std::string& filename);
    bool setupMesh(unsigned int ID, const aiMesh * mesh);
};