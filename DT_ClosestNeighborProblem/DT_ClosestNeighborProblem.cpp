// DT_ClosestNeighborProblem.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iomanip>
#include "Types.h"
#include "FileManager.h"
#include "GenerateData.h"
#include "ClosestNeighbors.h"
#include "RenderSDL.h"
#include <time.h>
//const int SCREEN_WIDTH = 640;
//const int SCREEN_HEIGHT = 480;
const char* unitTex = "C:/Users/user/source/repos/DT_ClosestNeighborProblem/res/circle.png";
const char* pathSettings = "Settings.xml";
const char* pathUnitData = "UnitData.xml";
int main(int argc, char* args[])
{
    //LoadSettings
    FileManager filemanager;
    Settings settings;
    if (!filemanager.LoadSettings(pathSettings, settings))
    {
        std::cout << "FileManager: LoadSettingsError" << std::endl;
        return 1;
    }
    //GenerateData
    GenerateData generateData(settings.screenWidth, settings.screenHeight, 8, 8);
    if (!generateData.generateUnitDataXML("UnitData.xml", 1000))
    {
        return 1;
    }
    //LoadData
    std::cout <<std::fixed<< std::setprecision(10);
    clock_t tStartLoad = clock();
    std::vector<UnitInfo> unitData;
    filemanager.LoadDataUnit(pathUnitData, unitData);
    std::cout << "LoadTime: " << (clock() - (double)tStartLoad) / CLOCKS_PER_SEC << std::endl;
    //Calculate
    ClosestNeighbors closestNeigbors(settings, 13);
    //Brute
    clock_t tStartCalculateBrute = clock();
    auto neighbors = closestNeigbors.FindNeighborsBrute(unitData);
    std::cout << "CalculteBruteTime: " << (clock() - (double)tStartCalculateBrute) / CLOCKS_PER_SEC << std::endl;
    for (size_t i = 0; i < unitData.size(); i++)
    {
        if (neighbors.find(i) != neighbors.end())
        {
            std::cout << "Unit " << i << ":" << neighbors[i].size() << std::endl;
        }
        else
        {
            std::cout << "Unit " << i << ":" << 0 << std::endl;
        }
    }
    //Нарисовать конус видимости и окрасить тех, кого видно в один цвет с обработанным агентом
    //Render
    RenderSDL renderSDL = RenderSDL(settings,13);
    //renderSDL.Draw(unitData, unitTex);
    //Получить код юнитам и нарисовать только их. У остальных не выделять ничего
    renderSDL.DrawNeigbors(unitData, unitTex, 5, neighbors[5],settings);
    renderSDL.DestroySDL();
    return 0;
}