
#include "Intelligence.h"

Intelligence::Intelligence()
    : map(nullptr)
{
    reset();
}

Intelligence::~Intelligence()
{
    SAFE_RELEASE(map);
}

void Intelligence::reset()
{
    tempDest = vec3(0.0, -1.0, 0.0);
    travel = 0.0f;
}

vec3 Intelligence::toSceneSpace(vec3 vector)
{
    return vec3(-1024.0, 0.0, -1024.0) + (vector * vec3(16.0, 16.0, 16.0));
}

bool Intelligence::applyUpdate(glm::vec3& position, FirstPersonCamera * camera)
{
    auto cameraPosition = camera->getPosition();
    auto velocity = vec3(0.0);
    auto makeVisible = true;
    
    if (tempDest.y != -1.0) 
    {
        if (glm::distance(position, tempDest) <= 5) 
        {
            tempDest = vec3(0.0, -1.0, 0.0);
        }
    }

    if (travel < 1.0) 
    {
        travel += (camera->getSpeed() / 2000.0); 
    }

    if (travel >= 1.0) 
    {
        PathPoint dest { (cameraPosition.x + 1024) / 16.0, (cameraPosition.z + 1024) / 16.0};

        if (tempDest.y == -1.0)
        {
            float div = (camera->getSpeed() / 15.0) * travel;
            map->findPath({ static_cast<int>(position.x), static_cast<int>(position.z) }, dest);
            velocity = (position + vec3(map->getPoint().x, 0.0, map->getPoint().y)) - position;

            if (camera->getSpeed() <= 0.01)     
            {
                makeVisible = false;
            }

            position.x += map->getPoint().x * div;
            position.z += map->getPoint().y * div;
        }
        else
        {
            PathPoint p1 = { static_cast<int>(position.x), static_cast<int>(position.z) };
            PathPoint p2 = { static_cast<int>(tempDest.x), static_cast<int>(tempDest.z) };

            map->findPath(p1, p2);

            velocity = (position + vec3(map->getPoint().x, 0.0, map->getPoint().y)) - position;

            position.x += map->getPoint().x * 0.05;
            position.z += map->getPoint().y * 0.05;
        }

        if (velocity.x >= 0.0) 
        {
            double target = glm::degrees(atan((velocity.z / velocity.x)));
            direction += (target - direction) / 10.0;
        } 
        else 
        {
            double target = (180.0f + glm::degrees(atan((velocity.z / velocity.x))));
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
        auto camera = vec3((p.x + 1024) / 16.0, 0.0, (p.z + 1024) / 16.0) + m;

        tempDest = camera;
        tempDest.y = 0.0;

        if (tempDest.x < 0) 
        {
            tempDest.x = 5;
        }
        
        if (tempDest.x > 127) 
        {
            tempDest.x = 125;
        }

        if (tempDest.z < 0) 
        {
            tempDest.z = 5;
        }
        
        if (tempDest.z > 127) 
        {
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
    if(!map)
    {
        map = new PathMap(package->newTexture("data/textures/ai_path2"));
    }
}

void Intelligence::end()
{
    map->clear();
}