
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

#include "Heightmap.h"

// SIDE IDs for each connect side to a heightmap surface
enum class SIDE {
    TOP, TOP_RIGHT, RIGHT, BOTTOM_RIGHT,
    BOTTOM, BOTTOM_LEFT, LEFT, TOP_LEFT
};

// Constructor & Deconstructor
Heightmap::Heightmap()
    : vertexCount(0), mesh(0), scale(1)
{
    plotTexture = NULL;
    overlayTexture = NULL;
}

Heightmap::~Heightmap()
{
    SAFE_RELEASE(mesh);
}

// returns the size of the heightmap as a 2D vector
glm::vec2 Heightmap::getHeightmapSize() 
{
    return (size);
}

// calculate the Y value at a certain pixel
GLfloat Heightmap::getY(FIBITMAP * sprite, int _x, int _y, float scale_y)
{
    // The colour of the pixel
    RGBQUAD colour;
    
    // get the pixels colour
    FreeImage_GetPixelColor(sprite, _x, _y, &colour);

    // then calculate the y value
    GLfloat y = 0.0f;

    y += ((GLfloat)colour.rgbGreen) / 255.0f;
    y += ((GLfloat)colour.rgbBlue) / 255.0f;
    y += ((GLfloat)colour.rgbRed) / 255.0f;

    // and multiply the result
    return (-2 + y*scale_y);
}

// returns the textures ID
GPU_ID Heightmap::getTexture()
{ 
    return texture->getID(); 
}

// returns the meshes ID
GPU_ID Heightmap::getVertexID() 
{ 
    return mesh->getID(); 
}

// returns the number of vertices in the mesh
GLuint Heightmap::getVertexCount()
{
    return vertexCount;
}

// calculates the normal for the surface
Vertex Heightmap::getSurfaceNormal(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3)
{
    return glm::cross(v2 - v1, v3 - v1);
}

// returns the height of the map at point x, y
GLfloat Heightmap::getHeightAt(int x, int y)
{
    int _x = x;
    int _y = y;

    // providing it points to a place on the height map
    if (_x >= 0 && _y >= 0 && _x < (size.x) && _y < (size.y))
    {
        // return the heights value
        return -(terrain_heights[_x][(size.y) - _y]);
    }
    else
    {
        // else return 0
        return 0.0;
    }
}

GLuint Heightmap::getOverlayID()
{
    return overlayTexture->getID();
}

GLuint Heightmap::getPathID()
{
    return plotTexture->getID();
}

// returns the connected surface to a certain surface
HeightmapSurface * Heightmap::getFace(SIDE side, GLuint i, GLuint b, GLuint size)
{
    // which side should we return
    switch (side) 
    {
        case SIDE::TOP:	if (b + 1 < size) { return faces[i][b + 1]; } break;
        case SIDE::TOP_RIGHT: if (i + 1 < size && b + 1 < size) { return faces[i + 1][b + 1]; } break;
        case SIDE::BOTTOM_RIGHT: if (i + 1 < size && b != 0) { return faces[i + 1][b - 1]; } break;
        case SIDE::BOTTOM_LEFT:if (i != 0 && b != 0){ return faces[i - 1][b - 1]; } break;
        case SIDE::TOP_LEFT: if (i != 0 && b + 1 < size) { return faces[i - 1][b + 1]; } break;
        case SIDE::RIGHT: if (i + 1 < size) { return faces[i + 1][b]; } break;
        case SIDE::BOTTOM: if (b != 0) return faces[i][b - 1];	break;
        case SIDE::LEFT: if (i != 0) return faces[i - 1][b]; else break;

        default: break;
    }

    // else return NULL
    return NULL;
}

// averages the normals across the surface
GLvoid Heightmap::averageNormals(std::vector<Vertex>& vertices)
{
    // for each surface in the surface
    for (int i = 0; i < faces.size(); i++) 
    {
        // ...
        for (int b = 0; b < faces.size(); b++) 
        {
            // acquire the surface
            HeightmapSurface * face = faces[i][b];
            // and average the normal across the 6 connected surfaces
            for (int z = 0; z < 6; z++) 
            { 
                // get the surface normal
                glm::vec3 normal = face->normal;
                // and the vertices for the normal
                glm::vec3 vert = face->vertices[z];
                // and add each surface normal that is connected to the surface
                for (int s = 0; s < 8; s++) 
                {
                    // get the related surface
                    HeightmapSurface * next = getFace((SIDE)s, i, b, faces.size());
                    
                    if (next != NULL) 
                    {
                        // check to see if it connects to each vertex
                        for (auto& v : next->vertices) 
                        {
                            // if it does
                            if (v == vert) 
                            {
                                // append the normal value
                                normal += next->normal;
                                break;
                            }
                        }
                    }
                }

                // then push back the new averaged normal into the vector
                vn.push_back(normal);
            }
        }
    }
}

GLvoid Heightmap::pushOverlay(TextureAsset * texture, TextureAsset * plotTextures)
{
    overlayTexture = new GPU_Sampler(SINGLE_SAMPLER);
    overlayTexture->setTransferQuality(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    overlayTexture->setBitmapData(texture->getPixels(),
        texture->getWidth(),
        texture->getHeight(),
        texture->getBPP(),
        texture->getMask()
    );

    overlayTexture->send();

    plotTexture = new GPU_Sampler(SINGLE_SAMPLER);
    plotTexture->setTransferQuality(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    plotTexture->setBitmapData(plotTextures->getPixels(),
        plotTextures->getWidth(),
        plotTextures->getHeight(),
        plotTextures->getBPP(),
        plotTextures->getMask()
    );

    plotTexture->send();
}

bool Heightmap::hasOverlay()
{
    return(plotTexture != NULL);
}

// This function creates the heightmap and fully sets it up for rendering
void Heightmap::create(TextureAsset * file, std::string heightmapFilename, float _x, float _y)
{
    scale = _x;

    // First load the bitmap where we grab the Y values from
    FIBITMAP * bitmap = FreeImage_Load(
        FreeImage_GetFIFFromFilename(heightmapFilename.c_str()), 
        heightmapFilename.c_str()
    );

    // Then calculate the width and height for the heightmap
    GLint textureHeight = FreeImage_GetHeight(bitmap);
    GLint textureWidth = FreeImage_GetWidth(bitmap);
    // then calculate the x and z positions
    GLint z = -textureHeight / 2;
    GLint x = -textureWidth / 2;

    // Set the size variable
    size = glm::vec2(textureWidth, textureHeight);

    // Create the transfer objects
    texture = new GPU_Sampler(SINGLE_SAMPLER);
    mesh = new GPU_Transfer();

    // and create a vector which will list each vertex in the mesh
    std::vector<Vertex> list;
    // and another which will list each uv cord
    std::vector<Vertex> uvs;

    // then do some pre allocation of memory to be more efficient when filling the vectors
    faces.reserve(textureWidth * textureHeight * 2);
    list.reserve(textureWidth * textureHeight * 2 * 3);
    uvs.reserve(textureWidth * textureHeight * 2 * 2);
    vn.reserve(textureWidth * textureHeight * 2 * 3);

    int uvx = 0;
    int uvy = 0;
    // then iterate through the entire texture creating 2 polygons for each pixel
    for (int i = 0; i < textureWidth+1; i++)
    {
        // push back a new float vector into the terrain heights vector
        terrain_heights.push_back(std::vector<float>());
        // and push pack a new height surface vector into the faces vector
        faces.push_back(std::vector<HeightmapSurface *>());

        // we create each surface going up the way through the image
        for (int a = 0; a < textureHeight + 1; a++, z++)
        {
            // calculate the heights at the following pixel
            float heights[] = {
                getY(bitmap, i, a, _y), 			getY(bitmap, i, a + 1, _y),
                getY(bitmap, i + 1, a + 1, _y), 	getY(bitmap, i, a, _y),
                getY(bitmap, i + 1, a + 1, _y),		getY(bitmap, i+1, a, _y)
            };

            // find its heighest height (used for collision detection)
            float heighest_height = 0.0;
            for (int b = 0; b < 6; b++) 
            {
                if (heights[b] * heights[b] > heighest_height*heighest_height) 
                {
                    heighest_height = heights[b];	
                }
            }

            // and push it back into the vector
            terrain_heights[i].push_back(heighest_height); 

            // push back the calculated mesh vertices into the vector
            list.push_back(Vertex(x,		heights[0], z - 1));
            list.push_back(Vertex(x,		heights[1], z));
            list.push_back(Vertex(x + 1,	heights[2], z));
            list.push_back(Vertex(x,		heights[3], z - 1));
            list.push_back(Vertex(x + 1,	heights[4], z));
            list.push_back(Vertex(x + 1,	heights[5], z - 1));

            // then push back the uv cords for that mesh
            uvs.push_back(Vertex(uvx, uvy, 0.0));
            uvs.push_back(Vertex(uvx, uvy+1, 0.0));
            uvs.push_back(Vertex(uvx+1, uvy+1, 0.0));
            uvs.push_back(Vertex(uvx, uvy, 0.0));
            uvs.push_back(Vertex(uvx+1, uvy+1, 0.0));
            uvs.push_back(Vertex(uvx+1, uvy, 0.0));
        
            // nor we create a new heightmap surface and get its surface normals
            auto surface = new HeightmapSurface();
            auto v1 = getSurfaceNormal(Vertex(x, heights[0], z - 1), Vertex(x, heights[1], z), Vertex(x + 1, heights[2], z));
            auto v2 = getSurfaceNormal(Vertex(x, heights[3], z - 1), Vertex(x + 1, heights[4], z), Vertex(x + 1, heights[5], z - 1));

            // we then back it into the faces array
            surface->normal = v1 + v2;
            surface->vertices = { 
                Vertex(x,		heights[0],		z - 1),
                Vertex(x,		heights[1],		z),
                Vertex(x + 1,	heights[2],		z),
                Vertex(x,		heights[3],		z - 1),
                Vertex(x + 1,	heights[4],		z),
                Vertex(x + 1,	heights[5],		z - 1)
            };

            faces[i].push_back(surface);
            ++uvy;
        }

        uvy = 0;
        uvx++;
        z = -textureHeight / 2;
        x++;
    }

    // we then scale the vertices by the given x_scale value if needed
    for (auto& v : list) {
        v.x *= _x;
        v.z *= _x;
    }

    // then average the normals for a realistic lighting model
    averageNormals(list);

    // we transfer this heightmap data to the gpu
    mesh->setTextureCords(uvs);
    mesh->setVertices(list);
    mesh->setNormals(vn);
    mesh->send();

    // then transfer the texture data for the heightmap
    texture->setTransferQuality(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    texture->setBitmapData(file->getPixels(),
        file->getWidth(),
        file->getHeight(),
        file->getBPP(),
        file->getMask()
    );

    texture->send();

    // and finally we unload the texture and get the number of vertices to be rendered
    vertexCount = list.size();
    FreeImage_Unload(bitmap);
}