#pragma once
#include <map>
#include <vector>
#include <functional> 
#include "Types.h"
#include <time.h>
#include <iostream>
#include <SDL.h>
#include "KDTree.hpp"
using indexArr = std::vector< size_t >;
class ClosestNeighbors
{
private:
	float sectorAngle;
	int sqDOV;
	std::vector<Edge> originPolygon;
	const float eps = 0.0001f;
	std::vector<Edge> CreateOriginPolygon(const int countEdge);
	// Поиск точек внутри сектора
	std::vector<Edge> RotatePolygon(const float sin, const float cos, const UnitInfo& unitInfo);
	bool inside_convex_polygon(const UnitInfo& unitInfo, const std::vector<Edge>& polygon);
	int get_side(const Edge& a, const Edge& b);
	float cosine_sign(const Edge& a, const Edge& b);
	Edge v_sub(const Edge& a, const Edge& b);
public:
	ClosestNeighbors(const Settings& settings,const int countEdge);
	std::map<size_t, indexArr> FindNeighborsBrute(const std::vector<UnitInfo>& dataUnit);
	std::map<size_t, indexArr> FindNeighborsKDTree(const std::vector<UnitInfo>& dataUnit);
	indexArr FindClosestPointInPolygon(const std::vector<Edge>& Polygon, const std::vector<UnitInfo>& dataUnit, const indexArr& neigbors);
};

