
#ifndef __WORLD_H__
#define __WORLD_H__

#include "EngineLayer.h"
#include "SolidCylinder.h"
#include "SolidBox.h"
#include "Camera.h"

struct BoxItem {
    std::function<void(Camera *, SolidBox *)>  func;
    SolidBox * box;
};

struct CylinderItem {
    std::function<void(Camera *, SolidCylinder *)>  func;
    SolidCylinder * cylinder;
};

class SILENCE_EXPORT World
{
private:
    std::vector<CylinderItem *> CylinderItems;
    std::vector<BoxItem *> BoxItems;
    Camera * camera;
public:
    World();
    ~World();

    void setPlayerCamera(Camera *);
    void updateWorld();

    void onHit(SolidCylinder *, std::function<void(Camera *, SolidCylinder *)>);
    void onHit(SolidBox *, std::function<void(Camera *, SolidBox *)>);
};

#endif