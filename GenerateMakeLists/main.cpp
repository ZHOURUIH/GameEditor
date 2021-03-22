#include "Generator.h"

int main()
{
	myMap<string, string> config = Generator::readConfig("./Config.txt");
	myVector<string> exceptDirList;
	StringUtility::split(config["ExcludeDir"].c_str(), ",", exceptDirList);
	myVector<string> dirList = Generator::generateDirectory(config["Path"], exceptDirList);

	// 修改CMakeLists.txt文件中的目录列表部分和源文件列表部分
	string cmakeListsPath = "./CMakeLists.txt";
	myVector<string> makeLists;
	FileUtility::openTxtFileLines(cmakeListsPath, makeLists, true);
	int dirStart = -1;
	int dirEnd = -1;
	FOR_VECTOR(makeLists)
	{
		if (makeLists[i] == "INCLUDE_DIRECTORIES(")
		{
			dirStart = i + 1;
		}
		if (dirStart != -1 && makeLists[i] == "IF(WIN32)")
		{
			dirEnd = i - 1;
			break;
		}
	}
	END(makeLists);

	if (dirStart == -1 || dirEnd == -1)
	{
		ERROR("CMakeLists.txt文件解析错误");
		system("pause");
		return 0;
	}

	// 修改目录列表
	makeLists.erase(dirStart, dirEnd - dirStart + 1);
	FOR_VECTOR(dirList)
	{
		makeLists.insert(dirStart + i, dirList[i] + ";");
	}
	string finalFile = StringUtility::stringsToString(makeLists.data(), makeLists.size(), "\r\n");
	finalFile = StringUtility::ANSIToUTF8(finalFile.c_str(), true);
	FileUtility::writeFileSimple(cmakeListsPath, finalFile);
	return 0;
}