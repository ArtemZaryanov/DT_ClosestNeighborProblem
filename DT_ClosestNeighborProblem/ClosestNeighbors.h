#pragma once
#include <map>
#include <vector>
#include <functional> 
#include "Types.h"
#include <SDL.h>
class ClosestNeighbors
{
private:
	float sectorAngle;
	int sqDOV;
	std::vector<Edge> originPolygon;
	std::vector<Edge> CreateOriginPolygon(const int countEdge);
	std::vector<Edge> RotatePolygon(const float sin, const float cos, const UnitInfo& unitInfo);
public:
	ClosestNeighbors(const Settings& settings,const int countEdge);
	std::map<int, std::vector<int>> FindNeighborsBrute(const std::vector<UnitInfo>& dataUnit);
	std::vector<int> FindClosestPointInPolygon(const std::vector<Edge>& Polygon, const std::vector<UnitInfo>& dataUnit, const std::vector<int>& neigbors);
};

