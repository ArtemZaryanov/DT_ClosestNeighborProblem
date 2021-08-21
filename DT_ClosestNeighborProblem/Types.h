#pragma once
#include<vector>
using indexArr = std::vector< size_t >;
struct UnitInfo
{
	//float sectorAngle;
	//int SqDistanceView;
	float directX;
	float directY;
	int posX;
	int posY;
	// Test
	int hash;
	int w;
	int h;
};
struct Point
{
	int x;
	int y;
};
struct Edge
{
	float x0;
	float y0;
	//int x1;
	//int y1;
};
struct Settings
{
	int screenWidth;
	int screenHeight;
	float sectorAngle;
	int SqDistanceView;
};
const float D2R = 1 / 57.29578f;
const float R2D = 1 / 0.017453f;