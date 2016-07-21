
#pragma once

#include "EngineLayer.h"

class A_StarMap;

struct A_StarPoint {
    int x; int y;
    A_StarPoint(int x, int y) {
        this->x = x; this->y = y;
    }
};

enum A_StarState {
    OPEN, CLOSED, START, END, PATH, DEAD
};

class SILENCE_EXPORT A_StarNode
{
private:
    A_StarPoint position;
    A_StarState state;
public:
    A_StarNode(int x, int y);
    ~A_StarNode();

    std::vector<A_StarNode *>& getAjacentNodes(A_StarMap * map);
    std::string toString();

    void setPosition(A_StarPoint point);
    void setState(RGBQUAD pixelColour);
    void setPosition(int x, int y);
    void setState(A_StarState);
    void close();
    void open();

    int getX();
    int getY();
    double getGain(A_StarPoint&);

    A_StarState getState();

    bool isOpen();
private:
    std::vector<A_StarNode *> ajacentNodes;
};