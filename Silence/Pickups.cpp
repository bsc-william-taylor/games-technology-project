
#include "Pickups.h"
#include "Player.h"

Pickups::Pickups() : 
    player(nullptr)
{
    position[0] = vec3(900, 2.0, 0);
    position[1] = vec3(-900, 2.0, 0);

    for (auto i = 0; i < 2; i++) 
    {
        draw[i] = true;

        pointLights[i].setPosition(position[i]);
        pointLights[i].setColour(glm::vec3(0.35, 0.2, 0.0));
        pointLights[i].setAttribuation(glm::vec4(0.1f, 0.3f, 0.007f, 0.0016f));
        pointLights[i].turnOn();
    }
}

Pickups::~Pickups()
{

}

void Pickups::create(LocalAssetManager * package, World& world, Player * player)
{
    this->player = player;

    pickup3.open(package->grabMusic("data/media/pickup3.mp3", LOAD));

    rocks[0].setModel(package->getM("data/models/rocks/obj"));
    rocks[1].setModel(package->getM("data/models/rocks/obj"));

    for (auto i = 0; i < 2; i++) 
    {
        auto mat = glm::mat4(1.0);    
        mat = glm::translate(mat, position[i]);
        mat = glm::scale(mat, glm::vec3(0.2, 0.2, 0.2));

        auto onHitCallback = [&](Camera * camera, SolidBox * box) {
            if (box->getMin().x < 0 && this->draw[1]) {
                this->pointLights[1].turnOff();
                this->player->replenishRocks();
                this->draw[1] = false;

                this->pickup3.reset();
                this->pickup3.play();
            }

            if (box->getMin().x > 0 && this->draw[0]) {
                this->pointLights[0].turnOff();
                this->player->replenishRocks();
                this->draw[0] = false;

                this->pickup3.reset();
                this->pickup3.play();
            }
        };

        world.onHit(rocks[i].createbox(mat), onHitCallback);
    }
}

void Pickups::getLights(Lights * lights)
{
    for (auto i = 0; i < 2; i++) 
    {
        lights->pushPointLight(&pointLights[i]);
    }
}

void Pickups::render(ForwardRenderer& renderer)
{
    for (auto i = 0; i < 2; i++) 
    {
        if (draw[i]) 
        {
            for (auto z = 0; z < 3; z++)
            {
                matrices.push();

                switch (z) {
                    case 0: matrices.translate(position[i] + glm::vec3(3.0, 0.0, 0.0)); break;
                    case 1: matrices.translate(position[i] + glm::vec3(0.0, 0.0, 0.0)); break;
                    case 2: matrices.translate(position[i] + glm::vec3(0.0, 0.0, 3.0)); break;

                    default: break;
                }

                matrices.scale(glm::vec3(0.05, 0.05, 0.05));

                renderer.setModelMatrix(matrices);
                renderer.renderModel(&rocks[i]);

                matrices.pop();
            }
        }
    }
}

void Pickups::update(Camera * camera)
{
}