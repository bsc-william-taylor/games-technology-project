
#pragma once

#include "EngineLayer.h"
#include "GPU.h"
#include "World.h"
#include "TextureAsset.h"

typedef GLfloat md2vec3[3];

/* MD2 header */
struct md2_header_t
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

/* Texture name */
struct md2_skin_t
{
    char name[64];
};

/* Texture coords */
struct md2_texCoord_t
{
    short s;
    short t;
};

/* Triangle info */
struct md2_triangle_t
{
    unsigned short vertex[3];
    unsigned short st[3];
};

/* Compressed vertex */
struct md2_vertex_t
{
    unsigned char v[3];
    unsigned char normalIndex;
};

/* Model frame */
struct md2_frame_t
{
    md2vec3 scale;
    md2vec3 translate;
    char name[16];
    struct md2_vertex_t *verts;
};

/* GL command packet */
struct md2_glcmd_t
{
    float s;
    float t;
    int index;
};

/* MD2 model structure */
struct md2_model_t
{
    struct md2_header_t header;

    struct md2_skin_t *skins;
    struct md2_texCoord_t *texcoords;
    struct md2_triangle_t *triangles;
    struct md2_frame_t *frames;
    int *glcmds;

    GLuint tex_id;
};

class SILENCE_EXPORT AnimatedModel
{
public:
    AnimatedModel();
    AnimatedModel(const char *fn0, const char * fn1);
    ~AnimatedModel();
    GLuint ReadMD2Model(const char *filename, const char * f);
    void FreeModel();
    void Animate(int animation, float dt);
    void Animate(float dt) { Animate(currentAnim, dt); }
    void ResetAnimation();
    int setCurrentAnim(int n);
    void Cycle();
private:
    md2_model_t mdl;
    int currentAnim;
    int currentFrame;
    int nextFrame;
    float interp;
    std::vector<GLfloat *> vertData;
    GLuint vertDataSize;
    GLfloat *animVerts;
    Vertices verts;
public:
    GLfloat* getAnimVerts() { return animVerts; }
    GLuint getVertDataSize() { return vertDataSize; }
    GLuint getVertDataCount() { return vertDataSize / 3; }
    GLuint getTextureID() { return sampler->getID(); }
    GLuint getVertexID() { return vertTransfer->getID(); }
    int getCurrentAnim() { return currentAnim; }
    SolidBox * createbox(glm::mat4);

    glm::vec3 getCenter(glm::mat4);
private:
    GPU_Transfer * vertTransfer;
    TextureAsset * texture;
    GPU_Sampler * sampler;
    Vertices vertices;
    Vertex min, max;
};