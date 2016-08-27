
#pragma once

#include "EngineLayer.h"

class PathMap;

struct PathPoint 
{
    int x; 
    int y;
};

enum class PathNodeState { Open, Closed, Start, End, Path, Dead };

class SILENCE_EXPORT PathNode
{
    std::vector<PathNode *> ajacentNodes;
    PathPoint position;
    PathNodeState state;
public:
    PathNode(int x, int y);
    ~PathNode();

    PathNodeState getState() const;

    std::vector<PathNode *>& getAjacentNodes(PathMap * map);
    std::string toString() const;

    double getGain(PathPoint&) const;

    void setPosition(PathPoint point);
    void setState(RGBQUAD pixelColour);
    void setPosition(int x, int y);
    void setState(PathNodeState);
    void close();
    void open();

    bool isOpen() const;

    int getX() const;
    int getY() const;
};