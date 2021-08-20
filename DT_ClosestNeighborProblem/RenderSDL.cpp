#include "RenderSDL.h"

RenderSDL::RenderSDL(const Settings& settings,const int countPoint)
{
    sqDOV = settings.SqDistanceView;
    sectorAngle = settings.sectorAngle;
    originPolygon = CreateOriginPolygon(countPoint);
    isInit = InitSDL(settings.screenWidth, settings.screenHeight);
}

bool RenderSDL::InitSDL(const int screenWidth, const int screenHeight)
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

    win = SDL_CreateWindow("Window", 100, 100, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
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

void RenderSDL::DrawUnit(SDL_Renderer*& ren, SDL_Texture*& tex, const SDL_Rect& r)
{
    SDL_RenderCopy(ren, tex, nullptr, &r);

}
void RenderSDL::DrawVisibleCone(SDL_Renderer*& ren, const UnitInfo& unitInfo, const Settings& settings)
{
    //SDL_Point* points = new SDL_Point[countPoint + 2];
    //SDL_Point r0{ unitInfo.posX, unitInfo.posY };
    //SDL_FPoint d{ unitInfo.directX, unitInfo.directY };
    //float dov = sqrtf((float)settings.SqDistanceView);
    //float sectorAngle = settings.sectorAngle * D2R;
    //float s = 0;
    //float c = 0;
    //points[0] = r0;
    //!
    /*for (int i = -countPoint / 2; i < 0; i++)
    {
        s = sinf(sectorAngle * i / countPoint);
        c = cosf(sectorAngle * i / countPoint);
        points[i + countPoint / 2 + 1].x = (r0.x + static_cast<int>(dov * (d.x * c - d.y * s)));
        points[i + countPoint / 2 + 1].y = (r0.y + static_cast<int>(dov * (d.y * c + d.x * s)));
    }
    if (countPoint % 2 != 0)
    {

        points[countPoint / 2 + countPoint % 2].x = (r0.x + static_cast<int>(dov * d.x));
        points[countPoint / 2 + countPoint % 2].y = (r0.y + static_cast<int>(dov * d.y));
    }
    for (int i = 1; i <= countPoint / 2; i++)
    {
        s = sinf(sectorAngle * i / countPoint);
        c = cosf(sectorAngle * i / countPoint);
        points[i + countPoint / 2 + countPoint % 2].x = (r0.x + static_cast<int>(dov * (d.x * c - d.y * s)));
        points[i + countPoint / 2 + countPoint % 2].y = (r0.y + static_cast<int>(dov * (d.y * c + d.x * s)));
    }
    points[countPoint + 1] = r0;*/
    //SDL_Point r0{ unitInfo.posX, unitInfo.posY };
    SDL_FPoint* points = RotatePolygon(unitInfo.directX, unitInfo.directY, unitInfo);
    //SDL_RenderDrawLine(ren, r0.x, r0.y, points[0].x, points[0].y);
    for (int i = 0; i < originPolygon.size()-1; i++)
    {
        SDL_RenderDrawLine(ren, points[i].x, points[i].y, points[i + 1].x, points[i + 1].y);
    }
    //SDL_RenderDrawLine(ren, points[countPoint+2].x, points[countPoint+2].y, r0.x, r0.y);
    //SDL_RenderDrawLinesF(ren, points, countPoint+2);
    delete[] points;
    points = nullptr;
}
SDL_FPoint* RenderSDL::RotatePolygon(const float sin, const float cos, const UnitInfo& unitInfo)
{
    SDL_FPoint* rotatingPolygon = new SDL_FPoint[originPolygon.size()];
    for (size_t i = 0; i < originPolygon.size(); i++)
    {
        rotatingPolygon[i] = SDL_FPoint{
            unitInfo.posX + originPolygon[i].x0 * cos - originPolygon[i].y0 * sin,
            unitInfo.posY + originPolygon[i].x0 * sin + originPolygon[i].y0 * cos };
    }
    return rotatingPolygon;
}
std::vector<Edge> RenderSDL::CreateOriginPolygon(const int countEdge)
{
    std::vector<Edge> polygon;
    SDL_FPoint* points = new SDL_FPoint[countEdge + 2];
    SDL_FPoint r0{ 0,0 };
    SDL_FPoint d{ 1, 0 };
    float dov = sqrtf((float)sqDOV);
    float sectorAngleRadians = sectorAngle * D2R;
    float s = 0;
    float c = 0;
    points[0] = r0;
    //!
    for (int i = -countEdge / 2; i < 0; i++)
    {
        s = sinf(sectorAngleRadians * i / countEdge);
        c = cosf(sectorAngleRadians * i / countEdge);
        points[i + countEdge / 2 + 1].x = (r0.x + dov * (d.x * c - d.y * s));
        points[i + countEdge / 2 + 1].y = (r0.y + dov * (d.y * c + d.x * s));
    }
    if (countEdge % 2 != 0)
    {

        points[countEdge / 2 + countEdge % 2].x = (r0.x + dov * d.x);
        points[countEdge / 2 + countEdge % 2].y = (r0.y + dov * d.y);
    }
    for (int i = 1; i <= countEdge / 2; i++)
    {
        s = sinf(sectorAngleRadians * i / countEdge);
        c = cosf(sectorAngleRadians * i / countEdge);
        points[i + countEdge / 2 + countEdge % 2].x = (r0.x + dov * (d.x * c - d.y * s));
        points[i + countEdge / 2 + countEdge % 2].y = (r0.y + dov * (d.y * c + d.x * s));
    }
    points[countEdge + 1] = r0;
    for (int i = 0; i <= countEdge + 1; i++)
    {
        polygon.push_back(
            {
                points[i].x,
                points[i].y,
                //points[i].x,
                //points[i].y,

            }
        );
    }
    delete[] points;
    points = nullptr;
    return polygon;
}
bool RenderSDL::DrawNeigbors(const std::vector<UnitInfo>& unitData, const char* fileTex, const int unit,const std::vector<int>& neighbors,const Settings& settings)
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
    SDL_Rect r;
    SDL_SetRenderDrawColor(ren, 0, 34, 23, 0);
    UnitInfo unitInfo;
    for (const int ind:neighbors)
    {
        unitInfo = unitData[ind];
        r.x = unitInfo.posX - unitInfo.w / 2;
        r.y = unitInfo.posY - unitInfo.h / 2;
        r.w = unitInfo.w;
        r.h = unitInfo.h;
        //Отправить структуру или в класс
        DrawUnit(ren, tex, r);
    }
    unitInfo = unitData[unit];
    r.x = unitInfo.posX - unitInfo.w / 2;
    r.y = unitInfo.posY - unitInfo.h / 2;
    r.w = unitInfo.w;
    r.h = unitInfo.h;
        //Отправить структуру или в класс
    SDL_SetRenderDrawColor(ren, 0, 34, 123, 0);
    DrawUnit(ren, tex, r);
    DrawVisibleCone(ren, unitInfo, settings);

    SDL_RenderPresent(ren);
    SDL_Delay(5000);
    return true;
}
bool RenderSDL::Draw(const std::vector<UnitInfo>& unitData, const char* fileTex, const Settings& settings)
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
    SDL_Rect r;
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
        SDL_RenderDrawLine(ren,
            r.x + r.w /2,
            r.y + r.h /2,
            r.x + r.w /2 + static_cast<int>(sqrtf((float)settings.SqDistanceView) * unitInfo.directX),
            r.y + r.h /2 + static_cast<int>(sqrtf((float)settings.SqDistanceView) * unitInfo.directY));
        DrawVisibleCone(ren, unitInfo,settings);
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
