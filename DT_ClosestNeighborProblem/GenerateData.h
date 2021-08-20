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
		int x0;
		int y0;
		int w;
		int h;
	};
	int width;
	int height;
	int cellWidth;
	int cellHeight;
	std::unordered_map<int, Cell> data;
	std::unordered_map<int, Cell> spaceUniformPartitioning();

public:
	GenerateData(const int width, const int height, const int cellWidth, const int cellHeight);
	std::vector<UnitInfo> generateUnitData(size_t count);
	bool  generateUnitDataXML(const char* file, size_t count);
};

