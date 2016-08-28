
#include "House.h"

House::House()
{
}

House::~House()
{
}

void House::create(LocalAssetManager * manager)
{
    house.setModel(manager->newModel("data/models/house/br_house1"));
}

void House::render(ForwardRenderer& renderer)
{
    matrices.push();
    matrices.scale(vec3(2.5, 2.0, 2.0));
    matrices.translate(vec3(0.0, -1.0, 0.0));
    matrices.rotate(-90.0f, vec3(1.0f, 0.0f, 0.0f));

    renderer.setModelMatrix(matrices);
    renderer.renderModel(&house);

    matrices.pop();
}

void House::update()
{
}