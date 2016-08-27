
#include "Key.h"

vector<vec3> keySpawnPoints = {
    vec3(-600.0, 2.0, 500.0),
    vec3(525.0, 2.0, 525.0),
    vec3(-800.0, 2.0, -100.0),
    vec3(-800, 2.0, -500),
    vec3(800, 2.0, 800),
    vec3(-200, 2.0, -700),
};

Key::Key() :
    taken(false)
{
    keyModel = nullptr;
    point = nullptr;
    spawnID = 0;
}

Key::~Key()
{
}

void Key::create(Model * key, LocalAssetManager * manager)
{
    pickup.open(manager->newAudio("data/media/pickup1.mp3", Load));
    keyModel = key;
}

void Key::spawn(World& world, PointLight * p)
{
    std::random_device randomDevice;
    std::mt19937 gen(randomDevice());
    std::uniform_int_distribution<> random(0, keySpawnPoints.size() - 1);

    spawnID = random(gen);
    point = p;
    taken = false;

    auto mat = glm::mat4(1.0);
    mat = glm::translate(mat, keySpawnPoints[spawnID]);
    mat = glm::rotate(mat, -90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    point->setPosition(keySpawnPoints[spawnID]);

    world.onHit(keyModel->createbox(mat), [&](Camera * c, SolidBox * b) {
        if (point != nullptr) {
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
    if (keyModel != nullptr && !taken)
    {
        matrices.push();
        matrices.translate(keySpawnPoints[spawnID]);
        matrices.rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

        renderer.setModelMatrix(matrices);
        renderer.renderModel(keyModel);

        matrices.pop();
    }
}