#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "Types.h"
#include <vector>
class RenderSDL
{
private:
	int SCREEN_WIDTH;;
	int SCREEN_HEIGHT;
	const float D2R = 1 / 57.29578f;
	const float R2D = 1 / 0.017453f;
	float sectorAngle;
	int sqDOV;
	SDL_Window* win;
	SDL_Renderer* ren;
	SDL_Texture* tex;
	std::vector<Edge> originPolygon;
	bool InitSDL(const int screenWidth, const int screenHeight);
	std::vector<Edge> CreateOriginPolygon(const int countEdge);
	SDL_FPoint*  RotatePolygon(const float sin, const float cos, const UnitInfo& unitInfo);
	bool isInit;
	//bool InitSDL(SDL_Window*& win, SDL_Renderer*& ren);
public:
	RenderSDL(const Settings& settings, const int countPoint);
	void DrawUnit(SDL_Renderer*& ren, SDL_Texture*& tex, const SDL_Rect& r);
	void DrawVisibleCone(SDL_Renderer*& ren, const UnitInfo& unitInfo, const Settings& settings);
	bool Draw(const std::vector<UnitInfo>& unitData, const char* fileTex, const Settings& settings);
	bool DrawNeigbors(const std::vector<UnitInfo>& unitData, const char* fileTex,const int unit,const std::vector<int>& neighbors, const Settings& settings);
	void DestroySDL();
};

