
#include "World.h"

World::World()
    : camera(nullptr)
{
}

World::~World()
{
    for (auto& item : cylinderItems) 
    {
        delete item->cylinder;
        delete item;
    }

    for (auto& item : boxItems) 
    {
        delete item->box;
        delete item;
    }

    cylinderItems.clear();
    boxItems.clear();
}

void World::setPlayerCamera(Camera * c)
{
    camera = c;
}

void World::updateWorld()
{
    if (camera != nullptr)
    {
        for (auto& item : boxItems)
        {
            if (item->box->collides(camera->getPosition()))
            {
                item->callback(camera, item->box);
            }
        }

        for (auto& item : cylinderItems)
        {
            if (item->cylinder->collides(camera->getPosition()))
            {
                item->callback(camera, item->cylinder);
            }
        }
    }
}

void World::onHit(SolidBox * box, BoxHitFunction callback)
{
    if (box != nullptr && callback != nullptr)
    {
        const auto worldItem = new BoxItem();
        worldItem->callback = callback;
        worldItem->box = box;
        boxItems.push_back(worldItem);
    }
}

void World::onHit(SolidCylinder * cy, CylinderHitFunction callback)
{
    if (cy != nullptr && callback != nullptr)
    {
        const auto worldItem = new CylinderItem();
        worldItem->callback = callback;
        worldItem->cylinder = cy;
        cylinderItems.push_back(worldItem);
    }
}