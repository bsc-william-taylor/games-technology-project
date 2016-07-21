
#include "World.h"

World::World()
{
    camera = NULL;
}

World::~World()
{
    for (auto& item : CylinderItems) {
        delete item->cylinder;
        delete item;
    }

    for (auto& item : BoxItems) {
        delete item->box;
        delete item;
    }

    CylinderItems.clear();
    BoxItems.clear();
}

void World::setPlayerCamera(Camera * c)
{
    camera = c;
}

void World::updateWorld()
{
    if (camera != NULL)
    {
        for (auto& item : BoxItems)
        {
            if (item->box->collides(camera->getPosition()))
            {
                item->func(camera, item->box);
            }
        }

        for (auto& item : CylinderItems)
        {
            if (item->cylinder->collides(camera->getPosition()))
            {
                item->func(camera, item->cylinder);
            }
        }
    }
}

void World::onHit(SolidBox * box, std::function<void(Camera *, SolidBox *)> func)
{
    if (box != NULL && func != NULL) 
    {
        BoxItem * worldItem = new BoxItem();
        worldItem->func = func;
        worldItem->box = box;
        BoxItems.push_back(worldItem);
    }
}

void World::onHit(SolidCylinder * cy, std::function<void(Camera *, SolidCylinder *)> func)
{
    if (cy != NULL && func != NULL)
    {
        CylinderItem * worldItem = new CylinderItem();
        worldItem->func = func;
        worldItem->cylinder = cy;
        CylinderItems.push_back(worldItem);
    }
}