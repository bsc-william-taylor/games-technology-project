
#pragma once

#include "EngineLayer.h"
#include "SolidCylinder.h"
#include "SolidBox.h"
#include "Camera.h"

using CylinderHitFunction = std::function<void(Camera *, SolidCylinder *)>;
using BoxHitFunction = std::function<void(Camera *, SolidBox *)>;

struct BoxItem 
{
    using Callback = BoxHitFunction;
    BoxHitFunction callback;
    SolidBox * box;
};

struct CylinderItem 
{   
    using Callback = CylinderHitFunction;
    CylinderHitFunction callback;
    SolidCylinder * cylinder;
};

class SILENCE_EXPORT World
{
    std::vector<CylinderItem *> cylinderItems;
    std::vector<BoxItem *> boxItems;

    Camera * camera;
public:
    World();
    virtual ~World();

    void onHit(SolidCylinder * cylinder, CylinderHitFunction callback);
    void onHit(SolidBox * box, BoxHitFunction callback);
    void setPlayerCamera(Camera * camera);
    void updateWorld();
};