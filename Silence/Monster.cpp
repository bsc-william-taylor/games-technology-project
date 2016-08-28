
#include "Monster.h"

Monster::Monster() : 
    enemy(nullptr), updated(false)
{
    intelligence.reset();
    spawnID = rand() % 4;

    switch (spawnID) 
    {
        case 0: 
            position = vec3(0.0, 0.0, 0.0); 
            break;
        case 1: 
            position = vec3(127, 0.0, 0.0); 
            break;
        case 2: 
            position = vec3(0.0, 0.0, 127); 
            break;
        case 3: 
            position = vec3(127, 0.0, 127); 
            break;
    
        default: 
            position = vec3(0.0, 0.0, 0.0);
            break;
    }

    enemyDirection = 0;
    alpha = 1.0;
}

Monster::~Monster()
{
    SAFE_RELEASE(enemy);
}

void Monster::create(LocalAssetManager * package)
{
    if(!enemy)
    {
        enemy = new AnimatedModel("data/models/enemy/archvile.md2", "data/models/enemy/archvile.png");

        intelligence.setup(package);
    }
}

void Monster::reset()
{
    switch (spawnID) {
        case 0: 
            position = vec3(0.0, 0.0, 0.0); 
            break;
        case 1: 
            position = vec3(127, 0.0, 0.0); 
            break;
        case 2: 
            position = vec3(0.0, 0.0, 127);
            break;
        case 3: 
            position = vec3(127, 0.0, 127); 
            break;

        default: 
            position = vec3(0.0, 0.0, 0.0);
            break;
    }

    intelligence.reset();
}

float Monster::getTravel()
{
    return intelligence.getTravel();
}

bool Monster::isCloseToPlayer(vec3 camera)
{
    auto monsterPosition = getPosition();
    monsterPosition.y = 0;

    cameraPosition = camera;
    cameraPosition.y = 0;

    return(glm::distance(cameraPosition, monsterPosition) <= 120.0 && alpha >= 0.5 && intelligence.getTravel() >= 1.0);
}

void Monster::update(Camera * camera)
{
    updated = intelligence.applyUpdate(position, static_cast<FirstPersonCamera *>(camera));
    enemyDirection = intelligence.getDirection();
}

bool Monster::hasKilledPlayer(glm::vec3 camera)
{
    auto monsterPosition = getPosition();
    monsterPosition.y = 0;

    cameraPosition = camera;
    cameraPosition.y = 0;

    return(glm::distance(cameraPosition, monsterPosition) <= 15.0 && alpha >= 0.5 && intelligence.getTravel() >= 1.0);
}

void Monster::distract(vec3 v)
{
    intelligence.setTemporaryDestination(cameraPosition, v);
}

vec3 Monster::to_scene_space(vec3 vector)
{
    return(vec3(-1024.0, 0.0, -1024.0) + (vector * vec3(16.0, 16.0, 16.0)));
}

bool Monster::isActive()
{
    return(intelligence.getTravel() >= 1.0);
}

vec3 Monster::getPosition()
{
    return to_scene_space(position);
}

void Monster::render(ForwardRenderer& renderer)
{
    matrices.push();
    matrices.translate(getPosition());
    matrices.rotate(intelligence.getDirection(), vec3(0.0, -1.0, 0.0));
    matrices.rotate(-90.0f, vec3(1.0, 0.0, 0.0));
    matrices.scale(vec3(0.5, 0.5, 0.5));

    if (updated) 
    {
        alpha = 1.0;
        enemy->animate(1, 0.1);
    }
    else 
    {
        alpha -= 0.01;
    }

    renderer.setAlpha(intelligence.getTravel() >= 1.0 ? alpha : 0.0);
    renderer.setModelMatrix(matrices);
    renderer.renderAnimatedModel(enemy);
    renderer.setAlpha(1.0);

    matrices.pop();
    intelligence.end();
}
