
#include "Monster.h"

Monster::Monster()
    : position(glm::vec3(0.0, 0.0, 0.0))
{
    intelligence.reset();
    spawnID = rand() % 4;

    switch (spawnID) {
        case 0: position = glm::vec3(0.0, 0.0, 0.0); break;
        case 1: position = glm::vec3(127, 0.0, 0.0); break;
        case 2: position = glm::vec3(0.0, 0.0, 127); break;
        case 3: position = glm::vec3(127, 0.0, 127); break;
    
        default: break;
    }

    enemyDirection = 0;
    alpha = 1.0;
}

Monster::~Monster()
{
}

void Monster::create(LocalAssetManager * package)
{
    enemy = new AnimatedModel("data/models/enemy/archvile.md2", "data/models/enemy/archvile.png");

    box = enemy->createbox(glm::mat4(1.0));

    intelligence.setup(package);
}

void Monster::reset()
{
    switch (spawnID) {
        case 0: position = glm::vec3(0.0, 0.0, 0.0); break;
        case 1: position = glm::vec3(127, 0.0, 0.0); break;
        case 2: position = glm::vec3(0.0, 0.0, 127); break;
        case 3: position = glm::vec3(127, 0.0, 127); break;

        default: break;
    }

    intelligence.reset();
}

float Monster::getTravel()
{
    return intelligence.getTravel();
}

bool Monster::isCloseToPlayer(glm::vec3 camera)
{
    glm::vec3 monsterPosition = getPosition();
    monsterPosition.y = 0;

    cameraPosition = camera;
    cameraPosition.y = 0;

    return(glm::distance(cameraPosition, monsterPosition) <= 120.0 && alpha >= 0.5 && intelligence.getTravel() >= 1.0);
}

void Monster::update(Camera * camera)
{
    updated = intelligence.applyUpdate(position, (FirstPersonCamera*)camera);
    enemyDirection = intelligence.getDirection();
}

bool Monster::hasKilledPlayer(glm::vec3 camera)
{
    glm::vec3 monsterPosition = getPosition();
    monsterPosition.y = 0;

    cameraPosition = camera;
    cameraPosition.y = 0;

    return(glm::distance(cameraPosition, monsterPosition) <= 15.0 && alpha >= 0.5 && intelligence.getTravel() >= 1.0);
}

void Monster::distract(glm::vec3 v)
{
    intelligence.setTemporaryDestination(cameraPosition, v);
}

glm::vec3 Monster::to_scene_space(glm::vec3 vector)
{
    return(glm::vec3(-1024.0, 0.0, -1024.0) + (vector * glm::vec3(16.0, 16.0, 16.0)));
}

bool Monster::isActive()
{
    return(intelligence.getTravel() >= 1.0);
}

glm::vec3 Monster::getPosition()
{
    return to_scene_space(position);
}

void Monster::render(ForwardRenderer& renderer)
{
    matrices.push();
    matrices.translate(getPosition());
    matrices.rotate(intelligence.getDirection(), glm::vec3(0.0, -1.0, 0.0));
    matrices.rotate(-90.0f, glm::vec3(1.0, 0.0, 0.0));
    matrices.scale(glm::vec3(0.5, 0.5, 0.5));

    if (intelligence.getTravel() >= 1.0) {
        renderer.setAlpha(alpha);
    } else {
        renderer.setAlpha(0.0);
    }

    if (updated) {
        alpha = 1.0;
        enemy->Animate(1, 0.1);
    }
    else {
        alpha -= 0.01;
    }

    renderer.setModelMatrix(matrices);
    renderer.renderAnimatedModel(enemy);
    renderer.setAlpha(1.0);

    matrices.pop();
    intelligence.end();
}
