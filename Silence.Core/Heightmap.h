
#pragma once

#include "EngineLayer.h"
#include "ModelFile.h"
#include "math.h"
#include "GPU.h"

struct HeightmapSurface 
{
    std::vector<Vertex> vertices;
    Vertex normal;
};

enum class SIDE;

class SILENCE_EXPORT Heightmap
{
    GPU_Sampler * overlayTexture;
    GPU_Sampler * plotTexture;
    GPU_Sampler * texture;
    GPU_Transfer * mesh;

    GpuID vertexCount;
public:
    Heightmap();
    ~Heightmap();

    glm::vec2 getHeightmapSize();

    GLvoid pushOverlay(TextureAsset * texture, TextureAsset * plotTextures);
    GLvoid create(TextureAsset * file, std::string, float _x, float _y);

    GLuint getOverlayID();
    GLuint getPathID();
    GLuint getVertexCount();
    GLuint getVertexID();
    GLuint getTexture();
   
    GLfloat getY(FIBITMAP * sprite, int x, int y, float scale_y);
    GLfloat getHeightAt(int x, int y);
    GLfloat getScale();


    bool hasOverlay();
private:
    std::vector<std::vector<HeightmapSurface *>> faces;
    std::vector<std::vector<float>> terrain_heights;
    std::vector<Vertex> vn;
    glm::vec2 size;

    HeightmapSurface * getFace(SIDE side, GLuint x, GLuint b, GLuint size);
    Vertex getSurfaceNormal(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);
    GLvoid averageNormals(std::vector<Vertex>& vertices);
    GLuint scale;
};