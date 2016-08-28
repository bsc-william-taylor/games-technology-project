#include "Heightmap.h"

Heightmap::Heightmap() : 
    vertexCount(0), 
    mesh(nullptr), 
    scale(1)
{
    overlayTexture = nullptr;
    plotTexture = nullptr;
    texture = nullptr;
}

Heightmap::~Heightmap()
{
    for(auto& faceList : faces)
    {
        for(auto& face : faceList)
        {
            delete face;
        }
    }

    SAFE_RELEASE(overlayTexture);
    SAFE_RELEASE(plotTexture);
    SAFE_RELEASE(texture);
    SAFE_RELEASE(mesh);
}

glm::vec2 Heightmap::getHeightmapSize() 
{
    return size;
}

GLfloat Heightmap::getY(FIBITMAP * sprite, int _x, int _y, float scale_y)
{
    RGBQUAD colour;
    FreeImage_GetPixelColor(sprite, _x, _y, &colour);

    auto y = 0.0f;
    y += float(colour.rgbGreen) / 255.0f;
    y += float(colour.rgbBlue) / 255.0f;
    y += float(colour.rgbRed) / 255.0f;
    return (-2 + y*scale_y);
}

GpuID Heightmap::getTexture()
{ 
    return texture->getID(); 
}

GpuID Heightmap::getVertexID() 
{ 
    return mesh->getID(); 
}

GLuint Heightmap::getVertexCount()
{
    return vertexCount;
}

Vertex Heightmap::getSurfaceNormal(vec3 v1, vec3 v2, vec3 v3)
{
    return glm::cross(v2 - v1, v3 - v1);
}

GLfloat Heightmap::getHeightAt(int x, int y)
{
    if (x >= 0 && y >= 0 && x < (size.x) && y < (size.y))
    {
        return -(terrain_heights[x][size.y - y]);
    }

    return 0.0;
}

GLuint Heightmap::getOverlayID()
{
    return overlayTexture->getID();
}

GLuint Heightmap::getPathID()
{
    return plotTexture->getID();
}

HeightmapSurface * Heightmap::getFace(HightmapSide side, GLuint i, GLuint b, GLuint size)
{
    switch (side) 
    {
        case HightmapSide::Top:	if (b + 1 < size) { return faces[i][b + 1]; } break;
        case HightmapSide::TopRight: if (i + 1 < size && b + 1 < size) { return faces[i + 1][b + 1]; } break;
        case HightmapSide::BottomRight: if (i + 1 < size && b != 0) { return faces[i + 1][b - 1]; } break;
        case HightmapSide::BottomLeft:if (i != 0 && b != 0){ return faces[i - 1][b - 1]; } break;
        case HightmapSide::TopLeft: if (i != 0 && b + 1 < size) { return faces[i - 1][b + 1]; } break;
        case HightmapSide::Right: if (i + 1 < size) { return faces[i + 1][b]; } break;
        case HightmapSide::Bottom: if (b != 0) return faces[i][b - 1];	break;
        case HightmapSide::Left: if (i != 0) return faces[i - 1][b]; else break;

        default: break;
    }

    return nullptr;
}

GLvoid Heightmap::averageNormals(std::vector<Vertex>& vertices)
{
    for (auto i = 0; i < faces.size(); i++) 
    {
        for (auto b = 0; b < faces.size(); b++)
        {
            const auto face = faces[i][b];

            for (auto z = 0; z < 6; z++)
            { 
                auto normal = face->normal;
                auto vert = face->vertices[z];

                for (auto s = 0; s < 8; s++)
                {
                    const auto next = getFace(HightmapSide(s), i, b, faces.size());
                    
                    if (next != nullptr) 
                    {
                        for (auto& v : next->vertices) 
                        {
                            if (v == vert) 
                            {
                                normal += next->normal;
                                break;
                            }
                        }
                    }
                }

                vn.push_back(normal);
            }
        }
    }
}

GLvoid Heightmap::pushOverlay(TextureAsset * texture, TextureAsset * plotTextures)
{
    if(!overlayTexture && !plotTexture)
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
}

bool Heightmap::hasOverlay()
{
    return(plotTexture != nullptr);
}

void Heightmap::create(TextureAsset * file, std::string heightmapFilename, float _x, float _y)
{
    if(texture && mesh)
    {
        return;
    }

    scale = _x;

    const auto imageType = FreeImage_GetFIFFromFilename(heightmapFilename.c_str());
    const auto bitmap = FreeImage_Load(imageType, heightmapFilename.c_str());

    GLint textureHeight = FreeImage_GetHeight(bitmap);
    GLint textureWidth = FreeImage_GetWidth(bitmap);
    auto z = -textureHeight / 2;
    auto x = -textureWidth / 2;

    texture = new GPU_Sampler(SINGLE_SAMPLER);
    mesh = new GPU_Transfer();
    size = vec2(textureWidth, textureHeight);

    vector<Vertex> list;
    vector<Vertex> uvs;

    faces.reserve(textureWidth * textureHeight * 2);
    list.reserve(textureWidth * textureHeight * 2 * 3);
    uvs.reserve(textureWidth * textureHeight * 2 * 2);
    vn.reserve(textureWidth * textureHeight * 2 * 3);

    auto uvx = 0;
    auto uvy = 0;

    for (auto i = 0; i < textureWidth+1; i++)
    {
        terrain_heights.push_back(vector<float>());
        faces.push_back(vector<HeightmapSurface *>());

        for (auto a = 0; a < textureHeight + 1; a++, z++)
        {
            float heights[] = {
                getY(bitmap, i, a, _y), 			getY(bitmap, i, a + 1, _y),
                getY(bitmap, i + 1, a + 1, _y), 	getY(bitmap, i, a, _y),
                getY(bitmap, i + 1, a + 1, _y),		getY(bitmap, i+1, a, _y)
            };

            auto heighest_height = 0.0;

            for (auto b = 0; b < 6; b++)
            {
                if (heights[b] * heights[b] > heighest_height*heighest_height) 
                {
                    heighest_height = heights[b];	
                }
            }

            terrain_heights[i].push_back(heighest_height); 

            list.push_back(Vertex(x,		heights[0], z - 1));
            list.push_back(Vertex(x,		heights[1], z));
            list.push_back(Vertex(x + 1,	heights[2], z));
            list.push_back(Vertex(x,		heights[3], z - 1));
            list.push_back(Vertex(x + 1,	heights[4], z));
            list.push_back(Vertex(x + 1,	heights[5], z - 1));

            uvs.push_back(Vertex(uvx, uvy, 0.0));
            uvs.push_back(Vertex(uvx, uvy+1, 0.0));
            uvs.push_back(Vertex(uvx+1, uvy+1, 0.0));
            uvs.push_back(Vertex(uvx, uvy, 0.0));
            uvs.push_back(Vertex(uvx+1, uvy+1, 0.0));
            uvs.push_back(Vertex(uvx+1, uvy, 0.0));
        
            auto surface = new HeightmapSurface();
            auto v1 = getSurfaceNormal(Vertex(x, heights[0], z - 1), Vertex(x, heights[1], z), Vertex(x + 1, heights[2], z));
            auto v2 = getSurfaceNormal(Vertex(x, heights[3], z - 1), Vertex(x + 1, heights[4], z), Vertex(x + 1, heights[5], z - 1));

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

    for (auto& v : list) 
    {
        v.x *= _x;
        v.z *= _x;
    }

    averageNormals(list);

    mesh->setTextureCords(uvs);
    mesh->setVertices(list);
    mesh->setNormals(vn);
    mesh->send();

    texture->setTransferQuality(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    texture->setBitmapData(file->getPixels(),
        file->getWidth(),
        file->getHeight(),
        file->getBPP(),
        file->getMask()
    );

    texture->send();
    vertexCount = list.size();
    FreeImage_Unload(bitmap);
}

GLfloat Heightmap::getScale() 
{
    return scale;
}