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
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const char* unitTex = "C:/Users/user/source/repos/DT_ClosestNeighborProblem/res/circle.png";
int main(int argc, char* args[])
{
    //GenerateData
    /*GenerateData generateData(SCREEN_WIDTH, SCREEN_HEIGHT, 4, 4);
    if (!generateData.generateUnitDataXML("UnitData.xml",300))
    {
        return 1;
    }*/
    //LoadData
    std::cout <<std::fixed<< std::setprecision(10);
    clock_t tStartLoad = clock();

    std::vector<UnitInfo> unitData;
    FileManager filemanager;
    filemanager.LoadDataUnit("UnitData.xml", unitData); 
    std::cout << "LoadTime: " << (double)(clock() - tStartLoad) / CLOCKS_PER_SEC << std::endl;
    //Calculate
    ClosestNeighbors closestNeigbors;
    //Brute
    clock_t tStartCalculateBrute = clock();
    auto neighbors = closestNeigbors.FindNeighborsBrute(unitData);
    std::cout << "CalculteBruteTime: " << (double)(clock() - tStartCalculateBrute) / CLOCKS_PER_SEC << std::endl;
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
    RenderSDL renderSDL = RenderSDL(SCREEN_WIDTH, SCREEN_HEIGHT);
    //renderSDL.Draw(unitData, unitTex);
    //Получить код юнитам и нарисовать только их. У остальных не выделять ничего
    renderSDL.DrawNeigbors(unitData, unitTex, 1, neighbors[1]);
    renderSDL.DestroySDL();
    return 0;
}