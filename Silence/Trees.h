
#pragma once

#include "InteractionLayer.h"
#include "ModelSet.h"

class Trees
{
    ModelSet treeModels;
    vector<glm::mat4> matrices;
public:
    Trees();
    ~Trees();

    void setup(LocalAssetManager *, Heightmap * map, World& world);
    void render(ForwardRenderer&);
};