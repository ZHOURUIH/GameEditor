#include <iostream>
#include "Utility.h"

std::string getSection(std::string& str, const std::string& key)
{
	if (str == "")
	{
		return "";
	}
	int pos = str.find_first_of(key);
	std::string section;
	if (pos != -1)
	{
		section = str.substr(0, pos);
		str = str.substr(pos + key.length(), str.length() - pos - key.length());
	}
	else
	{
		section = str;
	}
	return section;
}

void main()
{
	std::cout << "输入武器ID:";
	int weaponID = 0;
	std::cin >> weaponID;
	std::vector<std::string> fileList;
	Utility::findFiles("./", fileList, ".png", false);
	// 解析每个文件的信息
	int fileCount = fileList.size();
	for (int i = 0; i < fileCount; ++i)
	{
		std::string fileName = Utility::getFileName(fileList[i]);
		int direction = Utility::stringToInt(getSection(fileName, "_"));
		std::string action = getSection(fileName, "_");
		int index = Utility::stringToInt(getSection(fileName, "_"));
		// 拷贝到目标目录
		std::string destFolder = "weapon_id" + Utility::intToString(weaponID) + "_dir" + Utility::intToString(direction) + "_" + action;
		std::string destFileName = destFolder + "_" + Utility::intToString(index) + ".png";
		Utility::copyFile(fileList[i], destFolder + "/" + destFileName);
	}
}