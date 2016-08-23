
#include "Trees.h"

Trees::Trees()
{
}

Trees::~Trees()
{
}

void Trees::setup(LocalAssetManager * package, Heightmap * map, World& world)
{
    auto tree = package->getM("data/models/tree/tree1a_lod0");
    auto texture = package->getT("data/textures/pathways");

    treeModels.setModel(tree);

    for (auto x = 0; x < texture->getWidth(); x++) 
    {
        for (auto y = 0; y < texture->getHeight(); y++) 
        {
            auto colour = texture->getPixelColour(x, y);

            if (static_cast<GLfloat>(colour.rgbGreen) >= 200) {
                auto index = matrices.size();
                auto treeX = 64.0 - x;
                auto treeY = y - 64.0;
                auto sc = rand() % 5;

                matrices.push_back(glm::mat4());
                matrices[index] = glm::translate(matrices[index], vec3(-treeX, map->getHeightAt(x, y), treeY) * vec3(16, 1, 16));
                matrices[index] = glm::scale(matrices[index], vec3(5.0 + sc, 5.0 + (rand() % 5), 5.0 + sc));

                const auto hitCallback = [&](Camera * c, SolidCylinder * s) { c->cancelMovement(); };
                world.onHit(treeModels.createCylinder(matrices[index], 10.0), hitCallback);
            }
        }
    }

    treeModels.setMatrices({ matrices });
}

void Trees::render(ForwardRenderer& renderer) 
{
    renderer.renderModelSet(&treeModels);
}