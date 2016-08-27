
#include "Terrain.h"

Terrain::Terrain()
{
}

Terrain::~Terrain()
{
}

void Terrain::create(LocalAssetManager * package, World& world)
{
    terrain.create(package->newTexture("data/textures/grass"), "data/textures/heightmap.png", 16, 6);
    terrain.pushOverlay(package->newTexture("data/textures/dirt"), package->newTexture("data/textures/pathways"));

    trees.setup(package, &terrain, world);
    border.create(package, world);	
}

void Terrain::render(ForwardRenderer& renderer)
{
    trees.render(renderer);
    border.render(renderer);

    matrices.push();
    matrices.translate(glm::vec3(0.0, -7.0, 0.0));

    renderer.setModelMatrix(matrices);
    renderer.renderMap(&terrain);

    matrices.pop();
}

void Terrain::event(SDL_Event& e)
{
}

void Terrain::update(ForwardRenderer& renderer)
{
    renderer.handleTerrainHeight(&terrain);
    border.update();
}