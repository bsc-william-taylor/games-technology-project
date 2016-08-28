
#pragma once

#include "InteractionLayer.h"
#include "ModelSet.h"

class Trees
{
    vector<glm::mat4> matrices;
    ModelSet treeModels;
public:
    Trees();
    ~Trees();

    void setup(LocalAssetManager *, Heightmap * map, World& world);
    void render(ForwardRenderer&);
};