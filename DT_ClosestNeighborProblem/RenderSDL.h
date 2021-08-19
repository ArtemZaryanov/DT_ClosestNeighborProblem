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
	SDL_Window* win;
	SDL_Renderer* ren;
	SDL_Texture* tex;
	bool InitSDL(const int SCREEN_WIDTH, const int SCREEN_HEIGHT);
	bool isInit;
	//bool InitSDL(SDL_Window*& win, SDL_Renderer*& ren);
public:
	RenderSDL(const int SCREEN_WIDTH, const int SCREEN_HEIGHT);
	void DrawUnit(SDL_Renderer*& ren, SDL_Texture*& tex, const SDL_FRect& r);
	void DrawVisibleCone(SDL_Renderer*& ren, const UnitInfo& unitInfo, const int counPoint);
	bool Draw(const std::vector<UnitInfo>& unitData, const char* fileTex);
	bool DrawNeigbors(const std::vector<UnitInfo>& unitData, const char* fileTex,const int unit,const std::vector<int>& neighbors);
	void DestroySDL();
};

