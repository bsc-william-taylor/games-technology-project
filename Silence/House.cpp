
#include "House.h"

House::House()
{
}

House::~House()
{

}

void House::create(LocalAssetManager * manager)
{
	house.setModel(manager->getM("data/models/house/br_house1"));
}

void House::render(ForwardRenderer& renderer)
{
	matrices.push();
	// then set the initial model matrix
	matrices.scale(glm::vec3(2.5, 2.0, 2.0));
	matrices.translate(glm::vec3(0.0, -1.0, 0.0));
	matrices.rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

	// then render our house model
	renderer.setModelMatrix(matrices);
	renderer.renderModel(&house);

	matrices.pop();
}

void House::update()
{
}