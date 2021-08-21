#include "GenerateData.h"

std::unordered_map<int, GenerateData::Cell> GenerateData::spaceUniformPartitioning(){
    int N =  width / cellWidth;
    int M = height/ cellHeight;
    std::unordered_map<int, GenerateData::Cell> data;

    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            data.insert({ i + j * N, Cell{ j * cellHeight,i * cellHeight,cellWidth,cellHeight } });
        }
    }
    return data;
}

GenerateData::GenerateData(const int width, const int height, const int cellWidth, const int cellHeight)
{
        this->width = width;
        this->height = height;
        this->cellWidth = cellWidth;
        this->cellHeight = cellHeight;

}

std::vector<UnitInfo> GenerateData::generateUnitData(size_t count)
{
    data = spaceUniformPartitioning();
    std::vector<UnitInfo> unitData;
    Cell cell;
    int angle;
    size_t sizeData = data.size();
    for (const auto& uData:data)
    {
        if (unitData.size() == count)
        {
            break;
        }
        angle = rand();
        cell = uData.second;
        unitData.push_back(UnitInfo{ sinf((float)angle),cosf((float)angle),cell.x0 + cell.w / 2,cell.y0 + cell.h / 2,hash,cell.w,cell.h });
    }
    return unitData;
}

bool GenerateData::generateUnitDataXML(const char* file, size_t count)
{
    auto unitData = generateUnitData(count);
    tinyxml2::XMLDocument xml;
    tinyxml2::XMLNode* pRoot = xml.NewElement("UnitData");
    xml.InsertFirstChild(pRoot);
    tinyxml2::XMLElement* pElement;
    for (const auto& unit : unitData)
    {
        pElement = xml.NewElement("Unit");
        pElement->SetAttribute("hash", unit.hash);
        //pElement->SetAttribute("sectorAngle", unit.sectorAngle);
        //pElement->SetAttribute("distanceView", unit.SqDistanceView);
        pElement->SetAttribute("directX", unit.directX);
        pElement->SetAttribute("directY", unit.directY);
        pElement->SetAttribute("posX", unit.posX);
        pElement->SetAttribute("posY", unit.posY);
        pElement->SetAttribute("w", unit.w);
        pElement->SetAttribute("h", unit.h);
        pRoot->InsertEndChild(pElement);

    }

    tinyxml2::XMLError eResult = xml.SaveFile(file);
    if (eResult != tinyxml2::XMLError::XML_SUCCESS)
    {
        std::cout << "TinyXML:Don't create XML" << std::endl;
        return false;
    }
    return true;
}
