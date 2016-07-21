
#include "md2model.h"
#include "TextureAsset.h"
#include "HighPrecisionTimer.h"

/* Table of precalculated normals */
md2vec3 anorms_table[162] = {
    #include "anorms.h"
};

// Table of animation begin/end frames
static int animFrameList[40] = {
    0, 39, // stand
    1, 6, // run
    46, 53, // attack
    54, 57, //pain1
    58, 61, //pain2
    62, 65, //pain3
    66, 71, //jump
    72, 83, //flip
    84, 94, //salute
    95, 111, //taunt
    112, 122, //wave
    123, 134, //point
    135, 153, //crstnd
    154, 159, //crwalk
    160, 168, //crattak
    169, 172, //crpain
    173, 177, //crdeath
    178, 183, //death1
    184, 189, //death2
    190, 196 //death3
};


/*
// Table of animation begin/end frames
static int animFrameList[40] = {
    0, 39, // stand
    40, 45, // run
    46, 53, // attack
    54, 57, //pain1
    58, 61, //pain2
    62, 65, //pain3
    66, 71, //jump
    72, 83, //flip
    84, 94, //salute
    95, 111, //taunt
    112, 122, //wave
    123, 134, //point
    135, 153, //crstnd
    154, 159, //crwalk
    160, 168, //crattak
    169, 172, //crpain
    173, 177, //crdeath
    178, 183, //death1
    184, 189, //death2
    190, 196 //death3
};
*/

AnimatedModel::AnimatedModel()
{
    min = Vertex(1000000, 1000000, 1000000);
    max = Vertex(-1000000, -1000000, -1000000);
    currentAnim = 0;
    currentFrame = 0;
    nextFrame = 1;
    interp = 0.0f;	
}

AnimatedModel::AnimatedModel(const char * fn, const char * fn1)
{
    min = Vertex(1000000, 1000000, 1000000);
    max = Vertex(-1000000, -1000000, -1000000);
    ReadMD2Model(fn, fn1);
    currentAnim = 0;
    currentFrame = 0;
    nextFrame = 1;
    interp = 0.0f;
}

AnimatedModel::~AnimatedModel()
{
    FreeModel();
    int i;
    for (i=0;i<mdl.header.num_frames;++i) {
        delete [] vertData[i];
    }
    delete [] animVerts;
}

/**
* Load an MD2 model from file.
*
* Note: MD2 format stores model's data in little-endian ordering.  On
* big-endian machines, you'll have to perform proper conversions.
*/
GLuint AnimatedModel::ReadMD2Model(const char *filename, const char * nm)
{
    TextureAsset * texture = new TextureAsset("no-name");
    texture->grabFromFile(nm);
    
    sampler = new GPU_Sampler(SINGLE_SAMPLER);
    sampler->setTransferQuality(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    sampler->setBitmapWrapping(GL_REPEAT, GL_REPEAT);
    sampler->setBitmapData(texture->getPixels(),
        texture->getWidth(),
        texture->getHeight(),
        texture->getBPP(),
        texture->getMask()
    );

    sampler->send();

    FILE *fp;
    int i;

    fp = fopen (filename, "rb");
    if (!fp)
    {
        fprintf (stderr, "Error: couldn't open \"%s\"!\n", filename);
        return 0;
    }

    /* Read header */
    fread (&mdl.header, 1, sizeof (struct md2_header_t), fp);

    if ((mdl.header.ident != 844121161) ||
        (mdl.header.version != 8))
    {
        /* Error! */
        fprintf (stderr, "Error: bad version or identifier\n");
        fclose (fp);
        return 0;
    }

    /* Memory allocations */
    mdl.skins = (struct md2_skin_t *)malloc (sizeof (struct md2_skin_t) * mdl.header.num_skins);
    mdl.texcoords = (struct md2_texCoord_t *)malloc (sizeof (struct md2_texCoord_t) * mdl.header.num_st);
    mdl.triangles = (struct md2_triangle_t *)malloc (sizeof (struct md2_triangle_t) * mdl.header.num_tris);
    mdl.frames = (struct md2_frame_t *)malloc (sizeof (struct md2_frame_t) * mdl.header.num_frames);
    mdl.glcmds = (int *)malloc (sizeof (int) * mdl.header.num_glcmds);

    /* Read model data */
    fseek (fp, mdl.header.offset_skins, SEEK_SET);
    fread (mdl.skins, sizeof (struct md2_skin_t), mdl.header.num_skins, fp);

    fseek (fp, mdl.header.offset_st, SEEK_SET);
    fread (mdl.texcoords, sizeof (struct md2_texCoord_t), mdl.header.num_st, fp);

    fseek (fp, mdl.header.offset_tris, SEEK_SET);
    fread (mdl.triangles, sizeof (struct md2_triangle_t), mdl.header.num_tris, fp);

    fseek (fp, mdl.header.offset_glcmds, SEEK_SET);
    fread (mdl.glcmds, sizeof (int), mdl.header.num_glcmds, fp);

    /* Read frames */
    fseek (fp, mdl.header.offset_frames, SEEK_SET);
    for (i = 0; i < mdl.header.num_frames; ++i)
    {
        /* Memory allocation for vertices of this frame */
        mdl.frames[i].verts = (struct md2_vertex_t *)
            malloc (sizeof (struct md2_vertex_t) * mdl.header.num_vertices);

        /* Read frame data */
        fread (mdl.frames[i].scale, sizeof (md2vec3), 1, fp);
        fread (mdl.frames[i].translate, sizeof (md2vec3), 1, fp);
        fread (mdl.frames[i].name, sizeof (char), 16, fp);
        fread (mdl.frames[i].verts, sizeof (struct md2_vertex_t),
            mdl.header.num_vertices, fp);
    }

    fclose (fp);

    // now generate VBO data and create mesh
    // then save the data we actually need and free all the stuff we no longer need
    // this is required to allow the correct generation of normals etc

    int j;
    GLfloat s, t;
    md2vec3 v, *norm;
    struct md2_frame_t *pframe;
    struct md2_vertex_t *pvert;

    Vertices normals;
    Vertices tex_coords;

    pframe = &mdl.frames[0]; // first frame

    for (i = 0; i < mdl.header.num_tris; ++i)
    {
        // For each vertex 
        for (j = 0; j < 3; ++j)
        {
            tex_coords.push_back(glm::vec3((GLfloat)mdl.texcoords[mdl.triangles[i].st[j]].s / mdl.header.skinwidth, (GLfloat)mdl.texcoords[mdl.triangles[i].st[j]].t / mdl.header.skinheight, 0.0));

            // get current vertex
            pvert = &pframe->verts[mdl.triangles[i].vertex[j]];

            normals.push_back(glm::vec3(anorms_table[pvert->normalIndex][0], anorms_table[pvert->normalIndex][1], anorms_table[pvert->normalIndex][2]));
            // Vertices 
            // Doing these scaling operations *every* refresh is *very* wasteful
            // Should do all the scaling calculations once only, when loading the file
            //verts.push_back( pframe->scale[0] * pvert->v[0] + pframe->translate[0] );
            //verts.push_back( pframe->scale[1] * pvert->v[1] + pframe->translate[1] );
            //verts.push_back( pframe->scale[2] * pvert->v[2] + pframe->translate[2] );			
        }

        //normals.push_back(glm::vec3(0.0, 1.0, 0.0));
    }

    // now repeat for each frame...
    int k = 0;
    GLfloat *verts;
    vertDataSize = mdl.header.num_tris * 9;
    for (k=0;k<mdl.header.num_frames;++k) {
        verts = new GLfloat[vertDataSize];
        pframe = &mdl.frames[k]; // first frame
        for (i = 0; i < mdl.header.num_tris; ++i)
        {
            // For each vertex 
            for (j = 0; j < 3; ++j)
            {
                // get current vertex
                pvert = &pframe->verts[mdl.triangles[i].vertex[j]];
                
                verts[(i*3 + j)*3] = GLfloat(pframe->scale[0] * pvert->v[0] + pframe->translate[0]);
                verts[(i*3 + j)*3+1] = GLfloat(pframe->scale[1] * pvert->v[1] + pframe->translate[1]);
                verts[(i*3 + j)*3+2] = GLfloat(pframe->scale[2] * pvert->v[2] + pframe->translate[2]);
                
                int ID = vertices.size();
                vertices.push_back(glm::vec3(GLfloat(pframe->scale[0] * pvert->v[0] + pframe->translate[0]),
                    GLfloat(pframe->scale[1] * pvert->v[1] + pframe->translate[1]),
                    GLfloat(pframe->scale[2] * pvert->v[2] + pframe->translate[2]))
                );
            }

            int start = vertices.size() - 3;
            
            //normals.push_back(glm::cross(vertices[start+1] - vertices[start], vertices[start + 2] - vertices[start]));	//glm::cross(v2 - v1, v3 - v1);

            //normals.push_back(glm::cross(vertices[start] - vertices[start+1], vertices[start + 2] - vertices[start+1])); //glm::cross(v2 - v1, v3 - v1);
            //normals.push_back(glm::cross(vertices[start] - vertices[start+2], vertices[start + 1] - vertices[start+2]));
        }

        vertData.push_back(verts);
    }

    // initialise animVerts with frame 0 data
    animVerts = new GLfloat[vertDataSize];
    memcpy(animVerts,vertData[0],vertDataSize*sizeof(float));

    vertTransfer = new GPU_Transfer();
    vertTransfer->setTextureCords(tex_coords);
    vertTransfer->setVertices(vertices);
    vertTransfer->setNormals(normals);
    vertTransfer->send();

    this->verts.reserve(vertDataSize / 3);
    FreeModel();
    return NULL;
}

glm::vec3 AnimatedModel::getCenter(glm::mat4 mat)
{
    for (auto& vb : vertices)
    {
        glm::vec3 v = glm::vec3(mat * glm::vec4(vb, 1.0));

        if (v.x < min.x)	min.x = v.x;
        if (v.y < min.y)	min.y = v.y;
        if (v.z < min.z)	min.z = v.z;
        if (v.x > max.x)	max.x = v.x;
        if (v.y > max.y)	max.y = v.y;
        if (v.z > max.z)	max.z = v.z;
    }

    return(min + max) / glm::vec3(2, 2, 2);
}

SolidBox * AnimatedModel::createbox(glm::mat4 mat)
{
    SolidBox * solid = new SolidBox();
    for (auto& vb : vertices)
    {
        glm::vec3 v = glm::vec3(mat * glm::vec4(vb, 1.0));

        if (v.x < min.x)	min.x = v.x;
        if (v.y < min.y)	min.y = v.y;
        if (v.z < min.z)	min.z = v.z;
        if (v.x > max.x)	max.x = v.x;
        if (v.y > max.y)	max.y = v.y;
        if (v.z > max.z)	max.z = v.z;
    }
    
    solid->setPositionVector(min);
    solid->setSizeVector(max);
    return solid;
}

/**
* Free resources allocated for the model.
*/
void AnimatedModel::FreeModel()
{
    int i;

    if (mdl.skins)
    {
        free(mdl.skins);
        mdl.skins = NULL;
    }

    if (mdl.texcoords)
    {
        free(mdl.texcoords);
        mdl.texcoords = NULL;
    }

    if (mdl.triangles)
    {
        free(mdl.triangles);
        mdl.triangles = NULL;
    }

    if (mdl.glcmds)
    {
        free(mdl.glcmds);
        mdl.glcmds = NULL;
    }

    if (mdl.frames)
    {
        for (i = 0; i < mdl.header.num_frames; ++i)
        {
            free(mdl.frames[i].verts);
            mdl.frames[i].verts = NULL;
        }

        free(mdl.frames);
        mdl.frames = NULL;
    }
}



/**
* Calculate the current frame in animation, based on
* selected animation and current frame. If current frame
* is not in selected animation sequence, new current frame
* is start of animation sequence. 
* If interpolation is past 1.0, move current frame to next
* and next frame = current frame + 1
*/

void AnimatedModel::ResetAnimation()
{
    interp = 0.0;
}

void AnimatedModel::Animate(int animation, float dt)
{
    int start = animFrameList[animation * 2];
    int end = animFrameList[animation * 2 + 1];

    if ((currentFrame < start) || (currentFrame > end))
    {
        currentFrame = start;
        nextFrame = start + 1;
    }

    interp += dt;

    if (interp >= 1.0f)
    {

        // Move to next frame 
        interp = 0.0f;
        currentFrame = nextFrame;
        nextFrame++;

        if (nextFrame >= end+1)
            nextFrame = start;
    }


    if (interp == 0.0f)
    {
        memcpy(animVerts, vertData[currentFrame], vertDataSize*sizeof(float));
    }
    else 
    {		
        for (auto i = 0; i < vertDataSize; i+=3) {
            animVerts[i+0] = vertData[currentFrame][i+0] + interp*(vertData[nextFrame][i+0] - vertData[currentFrame][i+0]);
            animVerts[i+1] = vertData[currentFrame][i+1] + interp*(vertData[nextFrame][i+1] - vertData[currentFrame][i+1]);
            animVerts[i+2] = vertData[currentFrame][i+2] + interp*(vertData[nextFrame][i+2] - vertData[currentFrame][i+2]);
            
            verts.push_back(glm::vec3(animVerts[i], animVerts[i+1], animVerts[i+2]));
        }

        vertTransfer->setVertices(verts);
        vertTransfer->send();	
        verts.clear();
    }
}

void AnimatedModel::Cycle()
{
    currentFrame = 0;
    nextFrame = 1;
}

