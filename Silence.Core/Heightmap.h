
#pragma once

#include "EngineLayer.h"
#include "ModelFile.h"
#include "GPU.h"

struct HeightmapSurface 
{
    vector<Vertex> vertices;
    Vertex normal;
};

enum class HightmapSide
{
    Top, 
    TopRight, 
    Right, 
    BottomRight,
    Bottom,     
    BottomLeft,
    Left, 
    TopLeft
};

class SILENCE_EXPORT Heightmap
{
    GPU_Sampler * overlayTexture;
    GPU_Sampler * plotTexture;
    GPU_Sampler * texture;
    GPU_Transfer * mesh;
  
    vec2 size;

    GLuint vertexCount;
    GLuint scale;

    vector<vector<HeightmapSurface *>> faces;
    vector<vector<float>> terrain_heights;
    vector<Vertex> vn;
public:
    Heightmap();
    ~Heightmap();

    vec2 getHeightmapSize();

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
    HeightmapSurface * getFace(HightmapSide side, GLuint x, GLuint b, GLuint size);
    Vertex getSurfaceNormal(vec3 v1, vec3 v2, vec3 v3);
    GLvoid averageNormals(vector<Vertex>& vertices);
};