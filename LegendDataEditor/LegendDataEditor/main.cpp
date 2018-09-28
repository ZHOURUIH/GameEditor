#include <iostream>
#include "Utility.h"
#include "SQLite.h"

void main()
{
	std::cout << "选择功能(1:解析wix和wil文件. 2:分组角色动作):";
	int input;
	std::cin >> input;
	if (input == 1)
	{
		std::cout << "输入文件名:";
		std::string fileName;
		std::cin >> fileName;
		std::cout << "开始解析...";
		std::string filePath = "../media/" + fileName;
		std::string outputPath = "../media/" + fileName + "/";
		std::string WIX_SUFFIX = ".wix";
		std::string WIL_SUFFIX = ".wil";
		ImageUtility::wixWilToPNG(filePath + WIX_SUFFIX, filePath + WIL_SUFFIX, outputPath);
	}
	else if(input == 2)
	{
		std::cout << "输入文件夹名:";
		std::string fileName;
		std::cin >> fileName;
		std::cout << "开始解析..." << std::endl;
		long startTime = timeGetTime();
		std::string filePath = "../media/" + fileName;
		SQLite* mSQLite = TRACE_NEW(SQLite, mSQLite, "../media/MicroLegend.db");
		ImageUtility::groupImage(filePath, IT_HUMAN, mSQLite);
		TRACE_DELETE(mSQLite);
		std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
	}
	//system("pause");
	return;
}