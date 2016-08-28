
#include "Border.h"

Border::Border()   
    : borderArea(new SolidBox())
{
}

Border::~Border()
{
}

void Border::create(LocalAssetManager * package, World& world)
{
    wall.setModel(package->newModel("data/models/wall/wall"));

    borderArea->setPositionVector(vec3(-1020, -1020, -1020));
    borderArea->setSizeVector(vec3(1020, 1020, 1020));
    borderArea->reverse();

    const auto onHit = [&](Camera * camera, SolidBox *) { camera->cancelMovement(); };
    world.onHit(borderArea, onHit);
}

void Border::renderLeftAndRight(ForwardRenderer& renderer)
{
    matrices.push();
    matrices.scale(vec3(25, 25, 50));
    matrices.rotate(90, vec3(0.0, 1.0, 0.0));

    auto translateValue = vec3(-18, -0.2, 41);
 
    for (auto i = 0; i < 8; i++)	
    {
        matrices.translate(translateValue);
        renderer.setModelMatrix(matrices);
        renderer.renderModel(&wall);
        translateValue = vec3(5.5, 0.0, 0.0);
    }

    matrices.pop();
    matrices.push();
    matrices.scale(vec3(25, 25, 50));
    matrices.rotate(90, vec3(0.0, 1.0, 0.0));

    translateValue = vec3(-18, -0.2, -41);
 
    for (auto i = 0; i < 8; i++)	
    {
        matrices.translate(translateValue);
        renderer.setModelMatrix(matrices);
        renderer.renderModel(&wall);
        translateValue = vec3(5.5, 0.0, 0.0);
    }

    matrices.pop();
}

void Border::renderBackAndFront(ForwardRenderer& renderer)
{
    matrices.push();
    matrices.scale(vec3(50, 25, 25));

    auto translateValue = vec3(-18, -0.2, -41);

    for (auto i = 0; i < 8; i++)	
    {
        matrices.translate(translateValue);
        renderer.setModelMatrix(matrices);
        renderer.renderModel(&wall);
        translateValue = vec3(5.5, 0.0, 0.0);
    }

    matrices.pop();
    matrices.push();
    matrices.scale(vec3(185, 25, 25));
    matrices.translate(vec3(3.065, -0.2, 41));

    renderer.setModelMatrix(matrices);
    renderer.renderModel(&wall);

    matrices.translate(vec3(-6.13, 0.0, 0.0));

    renderer.setModelMatrix(matrices);
    renderer.renderModel(&wall);

    matrices.pop();
}

void Border::render(ForwardRenderer& renderer)
{
    renderLeftAndRight(renderer);
    renderBackAndFront(renderer);
}

void Border::update()
{
}