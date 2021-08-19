#include "FileManager.h"
FileManager::FileManager()
{
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
    // Переписать
    while (pElement != nullptr)
    {
        pElementAttribute = pElement->FirstAttribute();
        unitInfo.hash = atoi(pElementAttribute->Value());
        pElementAttribute = pElementAttribute->Next();
        unitInfo.sectorAngle = pElementAttribute->FloatValue();

        pElementAttribute = pElementAttribute->Next();
        unitInfo.SqDistanceView = pElementAttribute->FloatValue();

        pElementAttribute =pElementAttribute->Next();
        unitInfo.directX = pElementAttribute->FloatValue();

        pElementAttribute =pElementAttribute->Next();
        unitInfo.directY = pElementAttribute->FloatValue();

        pElementAttribute =pElementAttribute->Next();
        unitInfo.posX = pElementAttribute->FloatValue();

        pElementAttribute =pElementAttribute->Next();
        unitInfo.posY = pElementAttribute->FloatValue();

        pElementAttribute =pElementAttribute->Next();
        unitInfo.w = pElementAttribute->FloatValue();

        pElementAttribute =pElementAttribute->Next();
        unitInfo.h = pElementAttribute->FloatValue();
            //if (szAttributeText == nullptr) return XML_ERROR_PARSING_ATTRIBUTE;
        pElement = pElement->NextSiblingElement("Unit");
        unitData.push_back(unitInfo);
    }
    //if (pRoot == nullptr) return tinyxml2::XMLError::XML_ERROR_FILE_READ_ERROR

    return true;
}

void FileManager::SaveDataUnit(const std::string file)
{
}
