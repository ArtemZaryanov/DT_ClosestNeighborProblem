#include "RenderSDL.h"

RenderSDL::RenderSDL(const int SCREEN_WIDTH, const int SCREEN_HEIGHT)
{
    isInit = InitSDL(SCREEN_WIDTH, SCREEN_HEIGHT);
}

bool RenderSDL::InitSDL(const int SCREEN_WIDTH, const int SCREEN_HEIGHT)
{
    bool success = true;
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cout << "SDL:InitError:" << SDL_GetError() << std::endl;
        success = false;
    }
    int flags = IMG_INIT_PNG;
    if (!(IMG_Init(flags) & flags)) {
        std::cout << "Can't init image: " << IMG_GetError() << std::endl;
        return false;
    }

    win = SDL_CreateWindow("Window", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (win == nullptr)
    {
        std::cout << "SDL:CreateWindowError:" << SDL_GetError() << std::endl;
        success = false;
    }
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == nullptr) {
        std::cout << "SDL:CreateRendererError: " << SDL_GetError() << std::endl;
        success = false;
    }
    return success;
}

void RenderSDL::DrawUnit(SDL_Renderer*& ren, SDL_Texture*& tex, const SDL_FRect& r)
{
    SDL_RenderCopyF(ren, tex, nullptr, &r);

}
void RenderSDL::DrawVisibleCone(SDL_Renderer*& ren, const UnitInfo& unitInfo, const int countPoint)
{
    SDL_FPoint* points = new SDL_FPoint[countPoint + 2];
    SDL_FPoint r0{ unitInfo.posX, unitInfo.posY };
    SDL_FPoint d{ unitInfo.directX, unitInfo.directY };
    float dov = sqrtf(unitInfo.SqDistanceView);
    float sectorAngle = unitInfo.sectorAngle * D2R;
    float s = 0;
    float c = 0;
    points[0] = r0;
    //!
    for (int i = -countPoint / 2; i < 0; i++)
    {
        s = sinf(sectorAngle * i / countPoint);
        c = cosf(sectorAngle * i / countPoint);
        points[i + countPoint / 2 + 1].x = (r0.x + dov * (d.x * c - d.y * s));
        points[i + countPoint / 2 + 1].y = (r0.y + dov * (d.y * c + d.x * s));
    }
    if (countPoint % 2 != 0)
    {

        points[countPoint / 2 + countPoint % 2].x = (r0.x + dov * d.x);
        points[countPoint / 2 + countPoint % 2].y = (r0.y + dov * d.y);
    }
    for (int i = 1; i <= countPoint / 2; i++)
    {
        s = sinf(sectorAngle * i / countPoint);
        c = cosf(sectorAngle * i / countPoint);
        points[i + countPoint / 2 + countPoint % 2].x = (r0.x + dov * (d.x * c - d.y * s));
        points[i + countPoint / 2 + countPoint % 2].y = (r0.y + dov * (d.y * c + d.x * s));
    }
    points[countPoint + 1] = r0;
    SDL_RenderDrawLineF(ren, points[0].x, points[0].y, points[1].x, points[1].y);
    for (int i = 1; i < countPoint; i++)
    {
        SDL_RenderDrawLineF(ren, points[i].x, points[i].y, points[i + 1].x, points[i + 1].y);
    }
    SDL_RenderDrawLineF(ren, points[countPoint].x, points[countPoint].y, points[countPoint + 1].x, points[countPoint + 1].y);
    //SDL_RenderDrawLinesF(ren, points, countPoint+2);
    delete[] points;
    points = nullptr;
}
bool RenderSDL::DrawNeigbors(const std::vector<UnitInfo>& unitData, const char* fileTex, const std::vector<int>& neighbors)
{
    if (isInit != true)
    {
        DestroySDL();
        return false;
    }
    tex = IMG_LoadTexture(ren, fileTex);
    if (tex == nullptr) {
        std::cout << "SDL:CreateTextureFromSurfaceError: " << SDL_GetError() << std::endl;
        return false;
    }
    SDL_SetRenderDrawColor(ren, 0, 255, 255, 255);
    SDL_RenderClear(ren);
    SDL_RenderPresent(ren);
    SDL_FRect r;
    SDL_SetRenderDrawColor(ren, 0, 34, 255, 0);
    for (const auto& unitInfo : unitData)
    {
        //Поменять везде на 0.5? Или ввести доп. переменную типа пополам плюс к кэшированию!!!
        r.x = unitInfo.posX - unitInfo.w / 2;
        r.y = unitInfo.posY - unitInfo.h / 2;
        r.w = unitInfo.w;
        r.h = unitInfo.h;
        //Отправить структуру или в класс
        DrawUnit(ren, tex, r);
        SDL_RenderDrawLineF(ren,
            r.x + r.w * 0.5f,
            r.y + r.h * 0.5f,
            r.x + r.w * 0.5f + sqrt(unitInfo.SqDistanceView) * unitInfo.directX,
            r.y + r.h * 0.5f + sqrt(unitInfo.SqDistanceView) * unitInfo.directY);
        DrawVisibleCone(ren, unitInfo, 7);
        //SDL_SetRenderDrawColor(ren, 0, 34, 255, 0);

    }

    SDL_RenderPresent(ren);
    SDL_Delay(5000);
    return true;
}
bool RenderSDL::Draw(const std::vector<UnitInfo>& unitData, const char* fileTex)
{
    if (isInit != true)
    {
        DestroySDL();
        return false;
    }
    tex = IMG_LoadTexture(ren, fileTex);
    if (tex == nullptr) {
        std::cout << "SDL:CreateTextureFromSurfaceError: " << SDL_GetError() << std::endl;
        return false;
    }
    SDL_SetRenderDrawColor(ren, 0, 255, 255, 255);
    SDL_RenderClear(ren);
    SDL_RenderPresent(ren);
    SDL_FRect r;
    SDL_SetRenderDrawColor(ren, 0, 34, 255, 0);
    for (const auto& unitInfo : unitData)
    {
        //Поменять везде на 0.5? Или ввести доп. переменную типа пополам плюс к кэшированию!!!
        r.x = unitInfo.posX - unitInfo.w / 2;
        r.y = unitInfo.posY - unitInfo.h / 2;
        r.w = unitInfo.w;
        r.h = unitInfo.h;
        //Отправить структуру или в класс
        DrawUnit(ren, tex, r);
        SDL_RenderDrawLineF(ren,
            r.x + r.w * 0.5f,
            r.y + r.h * 0.5f,
            r.x + r.w * 0.5f + sqrt(unitInfo.SqDistanceView) * unitInfo.directX,
            r.y + r.h * 0.5f + sqrt(unitInfo.SqDistanceView) * unitInfo.directY);
        DrawVisibleCone(ren, unitInfo, 7);
        //SDL_SetRenderDrawColor(ren, 0, 34, 255, 0);
      
    }

    SDL_RenderPresent(ren);
    SDL_Delay(5000);
    return true;
}
void  RenderSDL::DestroySDL()
{
    SDL_DestroyWindow(win);
    win = nullptr;

    SDL_DestroyRenderer(ren);
    ren = nullptr;

    SDL_DestroyTexture(tex);
    tex = nullptr;

    SDL_Quit();
    IMG_Quit();
}
