
#include "Gate.h"
#include "ApplicationLayer.h"

Gate::Gate()
{
	exited = false;
}

Gate::~Gate()
{

}

void Gate::create(LocalAssetManager * package, World& world, SceneManager * scenes)
{
	this->scenes = scenes;
	gate.setModel(package->getM("data/models/gate/SecurityGate"));

	glm::mat4 mat = glm::mat4(1.0);

	mat = glm::scale(mat, glm::vec3(0.25, 0.25, 0.25));
	mat = glm::translate(mat, glm::vec3(0.0, 90.0, 0.0));
	mat = glm::rotate(mat, -90.0f, glm::vec3(0.0, -1.0, 0.0));
	mat = glm::translate(mat, glm::vec3(-4050.0, 0.0, 0.0));
	
	world.onHit(gate.createbox(mat), [&](Camera *, SolidBox *) {
		if (this->leave) {
			this->exited = true;
			this->scenes->switchScene((unsigned)SceneID::Gameover);
			this->leave = false;
		}
	});
}

bool Gate::hasHit()
{
	return exited;
}

void Gate::update(Key * key)
{
	leave = key->hasBeenPickedUp();
}

void Gate::render(ForwardRenderer& renderer)
{
	matrices.push();
	matrices.scale(glm::vec3(0.25, 0.25, 0.25));
	matrices.translate(glm::vec3(0.0, 90.0, 0.0));
	matrices.rotate(-90.0f, glm::vec3(0.0, -1.0, 0.0));
	matrices.translate(glm::vec3(-4100.0, 0.0, 0.0));
	
	renderer.setModelMatrix(matrices);
	renderer.renderModel(&gate);

	matrices.pop();
}

void Gate::reset()
{

}