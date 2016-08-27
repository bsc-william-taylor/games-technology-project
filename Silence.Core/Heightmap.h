
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

#ifndef __HEIGHTMAP__H_
#define __HEIGHTMAP__H_

#include "EngineLayer.h"
#include "ModelFile.h"
#include "math.h"
#include "GPU.h"

// The struct which represents a surface in the heightmap
struct HeightmapSurface 
{
    std::vector<Vertex> vertices;
    Vertex normal;
};

enum class SIDE;

// This class stores all the data needed for rendering a heightmap
class SILENCE_EXPORT Heightmap
{
private:
    // The sampler for the heightmaps texture
    GPU_Sampler * overlayTexture;
    // The sampler for the heightmaps texture
    GPU_Sampler * plotTexture;

    // The sampler for the heightmaps texture
    GPU_Sampler * texture;
    // The transfer for the heightmap mesh
    GPU_Transfer * mesh;
    // The number of vertices inside in mesh
    GpuID vertexCount;
public:
    // Constructor & Deconstructor
    Heightmap();
    ~Heightmap();

    // returns the heightmaps size as a 2D vector
    glm::vec2 getHeightmapSize();

    // creates the heightmap and sends all the data needed to the GPU 
    GLvoid create(TextureAsset * file, std::string, float _x, float _y);
    // returns the number of vertices
    GLuint getVertexCount();
    // returns the ID for the mesh
    GLuint getVertexID();
    // returns the ID for the texture
    GLuint getTexture();
    //
    GLvoid pushOverlay(TextureAsset * texture, TextureAsset * plotTextures);

    // Returns the y value for a specific pixel value
    GLfloat getY(FIBITMAP * sprite, int x, int y, float scale_y);
    // Returns the height of the generated heightmap at (x, y)
    GLfloat getHeightAt(int x, int y);

    GLfloat getScale() {
        return scale;
    }
    GLuint getOverlayID();
    GLuint getPathID();
    bool hasOverlay();
private:
    // a 2D vector of heightmap surfaces
    std::vector<std::vector<HeightmapSurface *>> faces;
    // a 2D vector of terrain heights
    std::vector<std::vector<float>> terrain_heights;
    // a vector of normals
    std::vector<Vertex> vn;
    // the size of the heightmap as a 2D vector
    glm::vec2 size;

    // returns the connected face
    HeightmapSurface * getFace(SIDE side, GLuint x, GLuint b, GLuint size);
    // returns the normal for a surface
    Vertex getSurfaceNormal(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);
    // averages the normals across the heightmaps surface
    GLvoid averageNormals(std::vector<Vertex>& vertices);
private:
    GLuint scale;
};

#endif