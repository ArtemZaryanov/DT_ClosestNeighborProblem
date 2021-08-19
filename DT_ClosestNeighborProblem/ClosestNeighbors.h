#pragma once
#include <map>
#include <vector>
#include <functional>
#include "Types.h"
#include <SDL.h>
class ClosestNeighbors
{
private:
	std::vector<Edge> CreatePolygon(const UnitInfo& unitInfo, const int countEdge);
public:
	std::map<int, std::vector<int>> FindNeighborsBrute(const std::vector<UnitInfo>& dataUnit);
	std::vector<int> FindClosestPointInPolygon(const std::vector<Edge>& Polygon, const std::vector<UnitInfo>& dataUnit, const std::vector<int>& neigbors);
};

