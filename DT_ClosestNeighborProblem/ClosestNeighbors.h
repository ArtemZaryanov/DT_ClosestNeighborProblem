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
	bool inside_convex_polygon(const UnitInfo& unitInfo, const std::vector<Edge>& polygon);
	int get_side(Edge a, Edge b);
	float cosine_sign(Edge a, Edge b);
	Edge v_sub(Edge a, Edge b);
public:
	ClosestNeighbors(const Settings& settings,const int countEdge);
	std::map<int, std::vector<int>> FindNeighborsBrute(const std::vector<UnitInfo>& dataUnit);
	std::vector<int> FindClosestPointInPolygon(const std::vector<Edge>& Polygon, const std::vector<UnitInfo>& dataUnit, const std::vector<int>& neigbors);
};

