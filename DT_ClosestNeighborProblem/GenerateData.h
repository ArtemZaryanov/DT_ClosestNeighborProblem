#pragma once
#include <iostream>
#include <unordered_map>
#include <set>
#include "tinyxml2.h"
#include "Types.h"
class GenerateData

{

private:
	struct Cell
	{
		float x0;
		float y0;
		float w;
		float h;
	};
	int width;
	int height;
	float cellWidth;
	float cellHeight;
	std::unordered_map<int, Cell> data;
	std::unordered_map<int, Cell> spaceUniformPartitioning();

public:
	GenerateData(const int width, const int height, const float cellWidth, const float cellHeight);
	std::vector<UnitInfo> generateUnitData(size_t count);
	bool  generateUnitDataXML(const char* file, size_t count);
};

