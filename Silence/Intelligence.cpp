
#include "Intelligence.h"

Intelligence::Intelligence()
{
    reset();
}

Intelligence::~Intelligence()
{

}

void Intelligence::reset()
{
    tempDest = glm::vec3(0.0, -1.0, 0.0);
    travel = 0.0f;
}

glm::vec3 Intelligence::to_scene_space(glm::vec3 vector)
{
    return(glm::vec3(-1024.0, 0.0, -1024.0) + (vector * glm::vec3(16.0, 16.0, 16.0)));
}

bool Intelligence::applyUpdate(glm::vec3& position, FirstPersonCamera * camera)
{
    bool makeVisible = true;
    glm::vec3 cameraPos = camera->getPosition();
    glm::vec3 vel = glm::vec3(0.0);
    
    if (tempDest.y != -1.0) {
        if (glm::distance(position, tempDest) <= 5) {
            tempDest = glm::vec3(0.0, -1.0, 0.0);
        }
    }

    if (travel < 1.0) {
        travel += (camera->getSpeed() / 4000.0); 
    }

    if (travel >= 1.0) 
    {
        A_StarPoint dest((cameraPos.x + 1024) / 16.0, (cameraPos.z + 1024) / 16.0);

        if (tempDest.y == -1.0)
        {
            float div = (camera->getSpeed() / 15.0) * travel;

            map->findPath(A_StarPoint(position.x, position.z), dest);

            vel = (position + glm::vec3(map->getPoint().x, 0.0, map->getPoint().y)) - position;

            if (camera->getSpeed() <= 0.01) {
                makeVisible = false;
            }

            position.x += map->getPoint().x * div;
            position.z += map->getPoint().y * div;
        }
        else
        {
            map->findPath(A_StarPoint(position.x, position.z), A_StarPoint(tempDest.x, tempDest.z));

            vel = (position + glm::vec3(map->getPoint().x, 0.0, map->getPoint().y)) - position;

            position.x += map->getPoint().x * 0.05;
            position.z += map->getPoint().y * 0.05;
        }

        if (vel.x >= 0.0) {
            double target = glm::degrees(atan((vel.z / vel.x)));
            direction += (target - direction) / 10.0;
        } else {
            double target = (180.0f + glm::degrees(atan((vel.z / vel.x))));
            direction += (target - direction) / 10.0;
        }
    }

    return makeVisible;
}

double Intelligence::getTravel()
{
    return travel;
}

void Intelligence::setTemporaryDestination(vec3 p, vec3 m)
{
    if (travel >= 1.0)
    {
        glm::vec3 camera = (glm::vec3((p.x + 1024) / 16.0, 0.0, (p.z + 1024) / 16.0)) + m;

        tempDest = camera;
        tempDest.y = 0.0;

        if (tempDest.x < 0) {
            tempDest.x = 5;
        }
        
        if (tempDest.x > 127) {
            tempDest.x = 125;
        }

        if (tempDest.z < 0) {
            tempDest.z = 5;
        }
        
        if (tempDest.z > 127) {
            tempDest.z = 125;
        }
    }
}

double Intelligence::getDirection()
{
    return direction;
}

void Intelligence::setup(LocalAssetManager * package)
{
    map = new A_StarMap(package->getT("data/textures/ai_path2"));
}

void Intelligence::end()
{
    map->clear();
}