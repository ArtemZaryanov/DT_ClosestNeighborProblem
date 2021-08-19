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
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;
const char* unitTex = "C:/Users/user/source/repos/DT_ClosestNeighborProblem/res/circle.png";
int main(int argc, char* args[])
{
    //GenerateData
    GenerateData generateData(SCREEN_WIDTH, SCREEN_HEIGHT, 5, 5);
    if (!generateData.generateUnitDataXML("UnitData.xml",10000))
    {
        return 1;
    }
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
    /*for (size_t i = 0; i < unitData.size(); i++)
    {
        if (neighbors.find(i) != neighbors.end())
        {
            std::cout << "Unit " << i << ":" << neighbors[i].size() << std::endl;
        }
        else
        {
            std::cout << "Unit " << i << ":" << 0 << std::endl;
        }
    }*/
    //���������� ����� ��������� � �������� ���, ���� ����� � ���� ���� � ������������ �������
    //Render
    RenderSDL renderSDL = RenderSDL(SCREEN_WIDTH, SCREEN_HEIGHT);
    renderSDL.Draw(unitData, unitTex);
    //�������� ��� ������ � ���������� ������ ��. � ��������� �� �������� ������

    renderSDL.DestroySDL();
    return 0;
}