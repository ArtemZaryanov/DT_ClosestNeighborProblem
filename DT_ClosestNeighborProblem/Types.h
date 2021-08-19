#pragma once
struct UnitInfo
{
	float sectorAngle;
	float SqDistanceView;
	float directX;
	float directY;
	float posX;
	float posY;
	// Test
	int hash;
	float w;
	float h;
};
struct Edge
{
	float x0;
	float y0;
	float x1;
	float y1;
};
const float D2R = 1 / 57.29578f;
const float R2D = 1 / 0.017453f;