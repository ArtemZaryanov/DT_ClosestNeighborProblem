#include "FileManager.h"
FileManager::FileManager()
{
}
bool FileManager::LoadSettings(const char* file, Settings& settings)
{
    tinyxml2::XMLDocument xml;
    tinyxml2::XMLError eResult = xml.LoadFile(file);
    if (eResult != tinyxml2::XMLError::XML_SUCCESS)
    {
        std::cout << "TinyXML:Don't create XML" << std::endl;
        return false;
    }
    tinyxml2::XMLNode* pRoot = xml.FirstChild();
    tinyxml2::XMLElement* pElement = pRoot->FirstChildElement();
    settings.screenHeight = pElement->FindAttribute("screenHeight")->IntValue();
    settings.screenWidth = pElement->FindAttribute("screenWidth")->IntValue();
    settings.sectorAngle = pElement->FindAttribute("sectorAngle")->FloatValue();
    settings.SqDistanceView = pElement->FindAttribute("SqDistanceView")->IntValue();
    return true;
}
bool FileManager::LoadDataUnit(const char* file, std::vector<UnitInfo>& unitData)
{
    tinyxml2::XMLDocument xml;
    tinyxml2::XMLError eResult = xml.LoadFile(file);
    UnitInfo unitInfo;
    if (eResult != tinyxml2::XMLError::XML_SUCCESS)
    {
        std::cout << "TinyXML:Don't create XML" << std::endl;
        return false;
    }
    tinyxml2::XMLNode* pRoot = xml.FirstChild();
    tinyxml2::XMLElement* pElement = pRoot->FirstChildElement();
    const tinyxml2::XMLAttribute* pElementAttribute;
    // ??????????
    while (pElement != nullptr)
    {
        pElementAttribute = pElement->FirstAttribute();
        unitInfo.hash = pElementAttribute->IntValue();

        pElementAttribute = pElementAttribute->Next();
        unitInfo.directX = pElementAttribute->FloatValue();

        pElementAttribute = pElementAttribute->Next();
        unitInfo.directY = pElementAttribute->FloatValue();

        pElementAttribute = pElementAttribute->Next();
        unitInfo.posX = pElementAttribute->IntValue();

        pElementAttribute = pElementAttribute->Next();
        unitInfo.posY = pElementAttribute->IntValue();

        pElementAttribute = pElementAttribute->Next();
        unitInfo.w = pElementAttribute->IntValue();

        pElementAttribute = pElementAttribute->Next();
        unitInfo.h = pElementAttribute->IntValue();
        //if (szAttributeText == nullptr) return XML_ERROR_PARSING_ATTRIBUTE;
        pElement = pElement->NextSiblingElement("Unit");
        unitData.push_back(unitInfo);
    }
    //if (pRoot == nullptr) return tinyxml2::XMLError::XML_ERROR_FILE_READ_ERROR

    return true;
}
bool FileManager::SaveDataUnit(std::map<size_t, indexArr> neighborsMap, const char* file)
{
    tinyxml2::XMLDocument xml;
    tinyxml2::XMLNode* pRoot = xml.NewElement("UnitNeighbors");
    xml.InsertFirstChild(pRoot);
    tinyxml2::XMLElement* pElement;
    //std::char unitNeighborsStr;
    for (const auto& neighbors : neighborsMap)
    {
        pElement = xml.NewElement("Unit");
        pElement->SetAttribute("countNeighbors", neighbors.second.size());
        /*pElement->FirstAttribute().
        unitNeighborsStr =
        for (const size_t neigh:neighbors.second)
        {
            pElemen
            unitNeighborsStr*/
            pElement->SetAttribute("neighbors", &*(neighbors.second.begin()));
        //    pElemen
        //}
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
