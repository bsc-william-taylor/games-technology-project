
#pragma once

#include "EngineLayer.h"
#include "GPU.h"
#include "World.h"
#include "TextureAsset.h"

typedef GLfloat MD2Vec3[3];

struct MD2Header
{
    int ident;
    int version;
    int skinwidth;
    int skinheight;
    int framesize;
    int num_skins;
    int num_vertices;
    int num_st;
    int num_tris;
    int num_glcmds;
    int num_frames;
    int offset_skins;
    int offset_st;
    int offset_tris;
    int offset_frames;
    int offset_glcmds;
    int offset_end;
};

struct MD2Skin
{
    char name[64];
};

struct MD2TexCoord
{
    short s;
    short t;
};

struct MD2Triangle
{
    unsigned short vertex[3];
    unsigned short st[3];
};

struct MD2Vertex
{
    unsigned char v[3];
    unsigned char normalIndex;
};

struct MD2Frame
{
    MD2Vec3 scale;
    MD2Vec3 translate;
    char name[16];
    struct MD2Vertex *verts;
};

struct MD2GLCommand
{
    float s;
    float t;
    int index;
};

struct MD2Model
{
    struct MD2Header header;
    struct MD2Skin *skins;
    struct MD2TexCoord *texcoords;
    struct MD2Triangle *triangles;
    struct MD2Frame *frames;
    int *glcmds;
    GLuint tex_id;
};

class SILENCE_EXPORT AnimatedModel
{
    GPU_Transfer * vertTransfer;
    GPU_Sampler * sampler;
    MD2Model mdl;

    GLfloat *animVerts;
    GLfloat interp;

    GLuint vertDataSize;
    GLuint currentAnim;
    GLuint currentFrame;
    GLuint nextFrame;

    TextureAsset * texture;

    Vertices vertices;
    Vertices verts;
    Vertex min, max;

    std::vector<GLfloat *> vertData;
public:
    AnimatedModel();
    AnimatedModel(const char * modal, const char * texture);
    ~AnimatedModel();

    void readModel(const char *filename, const char * f);
    void freeModel();
    void animate(int animation, float dt);
    void animate(float dt);
    void resetAnimation();
    void cycle();

    GLfloat * getAnimVerts();
    GLuint getVertDataSize();
    GLuint getVertDataCount();
    GLuint getTextureID();
    GLuint getVertexID();
    GLuint getCurrentAnim();

    SolidBox * createbox(glm::mat4);
    glm::vec3 getCenter(glm::mat4);
};