
#include "aStarNode.h"
#include "aStarMap.h"

A_StarNode::A_StarNode(int x, int y)
    : state(OPEN), position(A_StarPoint(x, y))
{
    ajacentNodes.reserve(8);
}

A_StarNode::~A_StarNode()
{
}

void A_StarNode::close()
{
    state = CLOSED;
}

void A_StarNode::setState(RGBQUAD pixelColour)
{
    if (static_cast<int>(pixelColour.rgbBlue) >= 70)
    { 
        state = A_StarState::OPEN;
    }
    else
    {
        state = A_StarState::CLOSED;
    }
}

std::string A_StarNode::toString()
{
    if (state == A_StarState::OPEN)
    {
        return " ";
    }
    else
    {
        return "X";
    }
}

bool A_StarNode::isOpen()
{
    return(state == OPEN || state == END);
}

double A_StarNode::getGain(A_StarPoint& point)
{
    int x = abs(point.x - position.x);
    int y = abs(point.y - position.y);

    return (sqrt(pow(x, 2) + pow(y, 2)));
}

void A_StarNode::setState(A_StarState state)
{
    this->state = state;
}

A_StarState A_StarNode::getState()
{
    return state;
}

void A_StarNode::open()
{
    state = OPEN;
}

int A_StarNode::getX()
{
    return position.x;
}

int A_StarNode::getY()
{
    return position.y;
}

void A_StarNode::setPosition(A_StarPoint point)
{
    position = point;
}

std::vector<A_StarNode *>& A_StarNode::getAjacentNodes(A_StarMap * map)
{
    ajacentNodes.clear();

    auto nodes = {
        map->get(position.x + 1, position.y+1),
        map->get(position.x + 1, position.y),
        map->get(position.x + 1, position.y-1),
        map->get(position.x, position.y + 1),
        map->get(position.x, position.y - 1),
        map->get(position.x - 1, position.y+1),
        map->get(position.x - 1, position.y),
        map->get(position.x - 1, position.y-1),
    };
    
    for (auto& element : nodes) {
        if (element != NULL) {
            ajacentNodes.push_back(element);
        }
    }

    return ajacentNodes;
}

void A_StarNode::setPosition(int x, int y)
{
    position.x = x;
    position.y = y;
}