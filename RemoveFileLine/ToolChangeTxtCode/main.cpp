// ToolChangeTxtCode.cpp : 定义控制台应用程序的入口点。
//
#include <iostream>

#include "Utility.h"

int main(int argc, char* argv[])
{
	// 打开文件
	char* buffer = NULL;
	int fileLength = 0;
	Utility::readFile("./output_log.txt", buffer, fileLength);
	std::vector<std::string> lineList;
	Utility::split(buffer, "\r\n", &lineList);
	// 去除无用行
	int lineCount = lineList.size();
	std::vector<std::string> resultList;
	for (int i = 0; i < lineCount; ++i)
	{
		if (Utility::startWith(lineList[i], "(Filename: C:/buildslave"))
		{
			continue;
		}
		else if (lineList[i].length() == 0)
		{
			continue;;
		}
		else if (lineList[i].length() == 1 && lineList[i][0] == ' ')
		{
			continue;
		}
		resultList.push_back(lineList[i]);
	}
	// 保存文件
	std::string finalStr;
	int resultCount = resultList.size();
	for (int i = 0; i < resultCount; ++i)
	{
		finalStr += resultList[i];
		finalStr += "\r\n";
	}
	Utility::writeFile("./log.txt", finalStr.length(), finalStr.c_str());
	system("pause");
	return 0;
}

