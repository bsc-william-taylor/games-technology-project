
#include "Border.h"

Border::Border()
{
    borderArea = new SolidBox();
}

Border::~Border()
{
    delete(borderArea);
    borderArea = NULL;
}

void Border::create(LocalAssetManager * package, World& world)
{
    wall.setModel(package->newModel("data/models/wall/wall"));

    borderArea->setPositionVector(glm::vec3(-1020, -1020, -1020));
    borderArea->setSizeVector(glm::vec3(1020, 1020, 1020));
    borderArea->reverse();

    world.onHit(borderArea, [&](Camera * camera, SolidBox *) {
        camera->cancelMovement();
    });
}

void Border::renderLeftAndRight(ForwardRenderer& renderer)
{

    matrices.push();
    matrices.scale(glm::vec3(25, 25, 50));
    matrices.rotate(90, glm::vec3(0.0, 1.0, 0.0));

    glm::vec3 translateValue = glm::vec3(-18, -0.2, 41); // glm::vec3(-39, -0.2, 20.5);
    for (int i = 0; i < 8; i++)	{
        
        matrices.translate(translateValue);

        renderer.setModelMatrix(matrices);
        renderer.renderModel(&wall);

        translateValue = glm::vec3(5.5, 0.0, 0.0);
    }

    matrices.pop();

    matrices.push();
    matrices.scale(glm::vec3(25, 25, 50));
    matrices.rotate(90, glm::vec3(0.0, 1.0, 0.0));

    translateValue = glm::vec3(-18, -0.2, -41); // glm::vec3(-39, -0.2, 20.5);
    for (int i = 0; i < 8; i++)	{

        matrices.translate(translateValue);

        renderer.setModelMatrix(matrices);
        renderer.renderModel(&wall);

        translateValue = glm::vec3(5.5, 0.0, 0.0);
    }

    matrices.pop();
}

void Border::renderBackAndFront(ForwardRenderer& renderer)
{

    matrices.push();
    matrices.scale(glm::vec3(50, 25, 25));

    glm::vec3 translateValue = glm::vec3(-18, -0.2, -41);
    for (int i = 0; i < 8; i++)	{
        matrices.translate(translateValue);

        renderer.setModelMatrix(matrices);
        renderer.renderModel(&wall);

        translateValue = glm::vec3(5.5, 0.0, 0.0);
    }

    
    matrices.pop();

    matrices.push();
    matrices.scale(glm::vec3(185, 25, 25));
    matrices.translate(glm::vec3(3.065, -0.2, 41));

    renderer.setModelMatrix(matrices);
    renderer.renderModel(&wall);

    matrices.translate(glm::vec3(-6.13, 0.0, 0.0));

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