#pragma once
#include "Types.h"
#include <string>
#include <vector>	
#include <tuple>
#include <fstream>
#include <iostream>
#include "tinyxml2.h"
class FileManager
{
public:
	FileManager();
	bool LoadDataUnit(const char* file, std::vector<UnitInfo>& unitData);
	void SaveDataUnit(const std::string file);
	



};

