
#include "PathMap.h"

PathMap::PathMap(TextureAsset * asset)
{
    directions.reserve(100);
    map.resize(asset->getWidth());

    for (auto x = 0; x < asset->getWidth(); x++) 
    {
        map[x].resize(asset->getHeight());
        for (auto y = 0; y < asset->getHeight(); y++) 
        {        
            map[x][y] = new PathNode(x, y);
            map[x][y]->setState(asset->getPixelColour(x, (asset->getHeight() - 1) - y));
        }
    }

    height = asset->getHeight();
    width = asset->getWidth();

    #ifdef PRINT_MAP
    std::ofstream stream("path.txt");
    for (auto y = 0; y < height; y++)
    {
        for (auto x = 0; x < width; x++)
        {
            stream.write(map[x][y]->toString().c_str(), 1);
        }

        stream.write("\n", 1);
    }
    #endif
}

PathMap::PathMap(int w, int h)
{
    directions.reserve(100);
    map.resize(w);

    for (auto x = 0; x < w; x++) 
    {
        map[x].resize(h);
        for (auto y = 0; y < h; y++) 
        {
            map[x][y] = new PathNode(x, y);
        }
    }

    height = h;
    width = w;
}

PathMap::~PathMap()
{
    for (auto x = 0; x < map.size(); x++) 
    {
        for (auto y = 0; y < map[x].size(); y++) 
        {
            delete map[x][y];
        }
    }
}

void PathMap::addWall(PathPoint start, PathPoint end)
{
    for (auto x = start.x; x <= end.x; x++) 
    {
        for (auto y = start.y; y <= end.y; y++)
        {
            map[x][y]->close();
        }
    }
}

PathNode * PathMap::get(int x, int y)
{
    if (x<0 || y<0 || x>=width || y>=height) 
    {
        return nullptr;
    } 

    if (map[x][y]->isOpen()) 
    {
        return map[x][y];
    } 

    return nullptr;
}

bool PathMap::checkAjacentNodes(PathPoint& point, PathPoint& destination)
{
    if (point.x != destination.x || point.y != destination.y)
    {
        PathNode * largestGainNode = nullptr;

        auto& ajacentNodes = map[point.x][point.y]->getAjacentNodes(this);
        
        for(auto& ajacentNode : ajacentNodes)
        {
            if (largestGainNode == nullptr)
            {
                largestGainNode = ajacentNode;
            }
            else if (largestGainNode->getGain(destination) > ajacentNode->getGain(destination))
            {
                largestGainNode = ajacentNode;
            }
        }

        if (largestGainNode == nullptr) 
        {
            return false;
        }

        auto newX = largestGainNode->getX();
        auto newY = largestGainNode->getY();

        directions.push_back({ newX - point.x, newY - point.y });

        point.x = newX;
        point.y = newY;
    }

    return true;
}

std::vector<PathPoint>& PathMap::getDirections()
{
    return directions;
}

void PathMap::printDirections(int limit)
{
    auto prints = 0;

    for (auto& direction : directions)
    {
        std::cout << direction.x << "," << direction.y << std::endl;

        if (++prints >= limit) 
        {
            break;
        }
    }
}

void PathMap::clear()
{
    directions.clear();

    for(auto& e : map) 
    {
        for(auto& e2 : e) 
        {
            if(e2->getState() == PathNodeState::Path) 
            {
                e2->setState(PathNodeState::Open);
            }
        }
    }
}

PathPoint PathMap::getPoint()
{
    return directions[0];
}

int PathMap::getHeight() const
{
    return height;
}

int PathMap::getWidth() const
{
    return width;
}

void PathMap::findPath(PathPoint start, PathPoint end)
{
    if (start.x >= 0 && start.y >= 0 && end.x >= 0 && end.y >= 0) 
    {
        if (start.x < width && start.y < height && end.x < width && end.y < height) 
        {
            map[start.x][start.y]->setState(PathNodeState::Start);
            map[end.x][end.y]->setState(PathNodeState::End);

            PathPoint current { start.x, start.y };

            while (true) 
            {
                if (checkAjacentNodes(current, end)) 
                {
                    if (map[current.x][current.y]->getState() == PathNodeState::End)
                    {
                        break;
                    } 
                        
                    map[current.x][current.y]->setState(PathNodeState::Path);
                }
                else 
                {
                    break;
                }
            }
        }
    }
}

void PathMap::print()
{
    for (auto x = 0; x < map.size(); x++) 
    {
        for (auto y = 0; y < map[x].size(); y++)
        {
            switch (map[y][x]->getState())
            {
                case PathNodeState::Closed: std::cout << "1 "; break;
                case PathNodeState::Start: std::cout << "S "; break;
                case PathNodeState::Path: std::cout << "X "; break;
                case PathNodeState::Dead: std::cout << "2 "; break;
                case PathNodeState::Open: std::cout << "0 "; break;
                case PathNodeState::End: std::cout << "E "; break;

                default: break;
            }
        }

        std::cout << std::endl;
    }
}