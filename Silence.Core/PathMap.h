
#pragma once

#include "PathException.h"
#include "PathNode.h"
#include "TextureAsset.h"

class SILENCE_EXPORT PathMap
{
    std::vector<std::vector<PathNode *>> map;
    std::vector<PathPoint> directions;
    int height;
    int width;
public:
    explicit PathMap(TextureAsset * asset);
    PathMap(int w, int h);
    virtual ~PathMap();

    PathNode * get(int x, int y);
    PathPoint getPoint();

    std::vector<PathPoint>& getDirections();

    void findPath(PathPoint start, PathPoint end);
    void addWall(PathPoint start, PathPoint end);
    void printDirections(int limit = 100);
    void print();
    void clear();

    int getHeight() const;
    int getWidth() const;
private:
    bool checkAjacentNodes(PathPoint& point, PathPoint& destination);
};