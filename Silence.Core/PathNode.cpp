
#include "PathNode.h"
#include "PathMap.h"

PathNode::PathNode(int x, int y) : 
    position({ x, y }),
    state(PathNodeState::Open)
{
    ajacentNodes.reserve(8);
}

PathNode::~PathNode()
{
}

void PathNode::close()
{
    state = PathNodeState::Closed;
}

void PathNode::setState(RGBQUAD pixelColour)
{
    if (static_cast<int>(pixelColour.rgbBlue) >= 70)
    { 
        state = PathNodeState::Open;
    }
    else
    {
        state = PathNodeState::Closed;
    }
}

std::string PathNode::toString() const
{
    return state == PathNodeState::Open ? " " : "X";
}

bool PathNode::isOpen() const
{
    return (state == PathNodeState::Open || state == PathNodeState::End);
}

double PathNode::getGain(PathPoint& point) const
{
    auto x = abs(point.x - position.x);
    auto y = abs(point.y - position.y);

    return (sqrt(pow(x, 2) + pow(y, 2)));
}

void PathNode::setState(PathNodeState state)
{
    this->state = state;
}

PathNodeState PathNode::getState() const
{
    return state;
}

void PathNode::open()
{
    state = PathNodeState::Open;
}

int PathNode::getX() const
{
    return position.x;
}

int PathNode::getY() const
{
    return position.y;
}

void PathNode::setPosition(PathPoint point)
{
    position = point;
}

std::vector<PathNode *>& PathNode::getAjacentNodes(PathMap * map)
{
    ajacentNodes.clear();

    auto nodes = 
    {
        map->get(position.x + 1, position.y+1),
        map->get(position.x + 1, position.y),
        map->get(position.x + 1, position.y-1),
        map->get(position.x, position.y + 1),
        map->get(position.x, position.y - 1),
        map->get(position.x - 1, position.y+1),
        map->get(position.x - 1, position.y),
        map->get(position.x - 1, position.y-1),
    };
    
    for(auto& node : nodes)
    {
        if(node != nullptr)
        {
            ajacentNodes.push_back(node);
        }
    }

    return ajacentNodes;
}

void PathNode::setPosition(int x, int y)
{
    position.x = x;
    position.y = y;
}