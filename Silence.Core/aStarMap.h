
#pragma once

#include "aStarException.h"
#include "aStarNode.h"
#include "TextureAsset.h"
#include <time.h>
#include <queue>

class A_StarMap 
{
private:
	std::vector<std::vector<A_StarNode *>> map;
	std::vector<A_StarPoint> directions;
	int height;
	int width;
public:
	A_StarMap(TextureAsset * asset);
	A_StarMap(int w, int h);
	~A_StarMap();

	A_StarPoint getPoint();

	std::vector<A_StarPoint>& getDirections();

	void findPath(A_StarPoint start, A_StarPoint end);
	void addWall(A_StarPoint start, A_StarPoint end);
	void printDirections(int limit = 100);
	void print();
	void clear();

	A_StarNode * get(int x, int y);

	int getHeight();
	int getWidth();
private:
	bool checkAjacentNodes(A_StarPoint& point, A_StarPoint& destination);
};