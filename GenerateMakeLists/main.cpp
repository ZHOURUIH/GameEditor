#include "Generator.h"

enum class SEARCH_STATE : byte
{
	NONE,
	DIRECTORY_LIST,
	FILE_LIST,
};

int main()
{
	myMap<string, string> config = Generator::readConfig("./Config.txt");
	myVector<string> exceptDirList;
	StringUtility::split(config["ExcludeDir"].c_str(), ",", exceptDirList);
	myVector<string> exceptFileList;
	StringUtility::split(config["ExcludeFile"].c_str(), ",", exceptFileList);
	myVector<string> dirList = Generator::generateDirectory(config["Path"], exceptDirList);
	myVector<string> fileList = Generator::generateSourceFile(config["Path"], exceptDirList, exceptFileList);

	// 修改CMakeLists.txt文件中的目录列表部分和源文件列表部分
	string cmakeListsPath = "./CMakeLists.txt";
	myVector<string> makeLists;
	FileUtility::openTxtFileLines(cmakeListsPath, makeLists, true);
	int dirStart = -1;
	int dirEnd = -1;
	int fileStart = -1;
	int fileEnd = -1;
	SEARCH_STATE state = SEARCH_STATE::DIRECTORY_LIST;
	FOR_VECTOR(makeLists)
	{
		if (state == SEARCH_STATE::DIRECTORY_LIST)
		{
			if (makeLists[i] == "INCLUDE_DIRECTORIES(")
			{
				dirStart = i + 1;
			}
			if (dirStart != -1 && makeLists[i] == "IF(WIN32)")
			{
				dirEnd = i - 1;
				state = SEARCH_STATE::FILE_LIST;
			}
		}
		else if (state == SEARCH_STATE::FILE_LIST)
		{
			if (makeLists[i] == "SET(SRC_LIST ")
			{
				fileStart = i + 1;
			}
			if (fileStart != -1 && makeLists[i] == ")")
			{
				fileEnd = i - 1;
				break;
			}
		}
	}
	END(makeLists);

	if (dirStart == -1 || dirEnd == -1 || fileStart == -1 || fileEnd == -1)
	{
		ERROR("CMakeLists.txt文件解析错误");
		system("pause");
		return 0;
	}

	// 先修改源文件列表,因为修改后不会影响前面的目录列表
	makeLists.erase(fileStart, fileEnd - fileStart + 1);
	FOR_VECTOR(fileList)
	{
		makeLists.insert(fileStart + i, fileList[i] + ";");
	}
	// 再修改目录列表
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