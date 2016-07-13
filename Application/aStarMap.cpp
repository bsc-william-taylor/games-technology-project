
#include "aStarMap.h"
#include "HighPrecisionTimer.h"

A_StarMap::A_StarMap(TextureAsset * asset)
{
	directions.reserve(100);
	map.resize(asset->getWidth());
	for (int x = 0; x < asset->getWidth(); x++) {
		map[x].resize(asset->getHeight());
		for (int y = 0; y < asset->getHeight(); y++) {
			map[x][y] = new A_StarNode(x, y);
			map[x][y]->setState(asset->getPixelColour(x, (asset->getHeight() - 1) - y));
		}
	}

	height = asset->getHeight();
	width = asset->getWidth();

	/*
	std::ofstream stream("path.txt");

	for (int y = 0; y < asset->getHeight(); y++) {
		for (int x = 0; x < asset->getWidth(); x++) {
			stream.write(map[x][y]->toString().c_str(), 1);
		}

		stream.write("\n", 1);
	}
	*/
}

A_StarMap::A_StarMap(int w, int h)
{
	directions.reserve(100);
	map.resize(w);
	for (int x = 0; x < w; x++) {
		map[x].resize(h);
		for (int y = 0; y < h; y++) {
			map[x][y] = new A_StarNode(x, y);
		}
	}

	height = h;
	width = w;
}

A_StarMap::~A_StarMap()
{
	for (int x = 0; x < map.size(); x++) {
		for (int y = 0; y < map[x].size(); y++) {
			delete(map[x][y]);
		}
	}
}

void A_StarMap::addWall(A_StarPoint start, A_StarPoint end)
{
	for (int x = start.x; x <= end.x; x++) {
		for (int y = start.y; y <= end.y; y++) {
			map[x][y]->close();
		}
	}
}

A_StarNode * A_StarMap::get(int x, int y)
{
	if (x<0 || y<0 || x>=width || y>=height) {
		return NULL;
	} else {
		if (map[x][y]->isOpen()) {
			return map[x][y];
		} else {
			return NULL;
		}
	}
}

bool A_StarMap::checkAjacentNodes(A_StarPoint& point, A_StarPoint& destination)
{
	if (point.x != destination.x || point.y != destination.y)
	{
		A_StarNode * largestGainNode = NULL;
		//try
		{
			auto& ajacentNodes = map[point.x][point.y]->getAjacentNodes(this);

			for (int i = 0; i < ajacentNodes.size(); i++) {
				if (largestGainNode == NULL) {
					largestGainNode = ajacentNodes[i];
				} else if (largestGainNode->getGain(destination) > ajacentNodes[i]->getGain(destination)) {
					largestGainNode = ajacentNodes[i];
				}
			}

			if (largestGainNode == NULL) {
				return false;
			//	throw A_StarException("Path Got Trapped!!!");
			}

			int newX = largestGainNode->getX();
			int newY = largestGainNode->getY();

			directions.push_back(A_StarPoint(newX - point.x, newY - point.y));

			point.x = newX;
			point.y = newY;
		}
		//catch (A_StarException e)
		{
			//std::cout << e.what() << std::endl;
			//std::cin.get();
			//std::exit(0);
		}
	}

	return true;
}

std::vector<A_StarPoint>& A_StarMap::getDirections()
{
	return directions;
}

void A_StarMap::printDirections(int limit)
{
	int prints = 0;

	for (auto& direction : directions)
	{
		std::cout << direction.x << "," << direction.y << std::endl;
		if (++prints >= limit) {
			break;
		}
	}
}

void A_StarMap::clear()
{
	directions.clear();
	for(auto& e : map) {
		for(auto& e2 : e) {
			if(e2->getState() == PATH) {
				e2->setState(OPEN);
			}
		}
	}
}

A_StarPoint A_StarMap::getPoint()
{
	return directions[0];
}

int A_StarMap::getHeight()
{
	return height;
}

int A_StarMap::getWidth()
{
	return width;
}

void A_StarMap::findPath(A_StarPoint start, A_StarPoint end)
{
	if (start.x >= 0 && start.y >= 0 && end.x >= 0 && end.y >= 0) {
		if (start.x < width && start.y < height && end.x < width && end.y < height) {
			/*
			if (map[end.x][end.y]->getState() == CLOSED) {
				std::cout << "Closed Destination " << end.x << ":" << end.y << std::endl;
			}
			*/

			map[start.x][start.y]->setState(START);
			map[end.x][end.y]->setState(END);

			A_StarPoint current = A_StarPoint(start.x, start.y);

			while (1) {
				if (checkAjacentNodes(current, end)) {
					if (map[current.x][current.y]->getState() == END) {
						break;
					} else {
						map[current.x][current.y]->setState(A_StarState::PATH);
					}
				}
				else {
					break;
				}
			}
		}
	}
}

void A_StarMap::print()
{
	for (int x = 0; x < map.size(); x++) {
		for (int y = 0; y < map[x].size(); y++) {
			switch (map[y][x]->getState())
			{
				case START: std::cout << "S "; break;
				case PATH: std::cout << "X "; break;
				case CLOSED: std::cout << "1 "; break;
				case DEAD: std::cout << "2 "; break;
				case OPEN: std::cout << "0 "; break;
				case END: std::cout << "E "; break;

				default: break;
			}
		}

		std::cout << std::endl;
	}
}