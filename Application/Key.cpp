
#include "Key.h"

std::vector<glm::vec3> keySpawnPoints = {
	glm::vec3(-600.0, 2.0, 500.0),
	glm::vec3(525.0, 2.0, 525.0),
	glm::vec3(-800.0, 2.0, -100.0),
	glm::vec3(-800, 2.0, -500),
	glm::vec3(800, 2.0, 800),
	glm::vec3(-200, 2.0, -700),
};

Key::Key()
{
	point = NULL;
	keyModel = NULL;
	spawnID = 0;
}

Key::~Key()
{

}

void Key::create(Model * key, LocalAssetManager * manager)
{
	pickup.open(manager->grabMusic("data/media/pickup1.mp3", LOAD));
	keyModel = key;
}

void Key::spawn(World& world, PointLight * p)
{
	point = p;
	taken = false;

	std::random_device randomDevice;
	std::mt19937 gen(randomDevice());
	std::uniform_int_distribution<> random(0, keySpawnPoints.size()-1);
	spawnID = 0;//random(gen);

	glm::mat4 mat = glm::mat4(1.0);

	mat = glm::translate(mat, keySpawnPoints[spawnID]);
	mat = glm::rotate(mat, -90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

	point->setPosition(keySpawnPoints[spawnID]);

	world.onHit(keyModel->createbox(mat), [&](Camera * c, SolidBox * b) {
		if (point != NULL) {
			pickup.reset();
			pickup.play();
			point->turnOff();
			taken = true;
		}
	});
}

void Key::reset()
{
	point->turnOn();
	taken = false;
}

glm::vec3 Key::getPosition()
{
	return keySpawnPoints[spawnID];
}

bool Key::hasBeenPickedUp()
{
	return taken;
}

void Key::update()
{

}

void Key::render(ForwardRenderer& renderer)
{
	if (keyModel != NULL && !taken)
	{
		matrices.push();
		matrices.translate(keySpawnPoints[spawnID]);
		matrices.rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

		renderer.setModelMatrix(matrices);
		renderer.renderModel(keyModel);

		matrices.pop();
	}
}