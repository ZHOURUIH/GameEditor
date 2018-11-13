#include <iostream>
#include "Utility.h"
#include "SQLite.h"

void main()
{
	while (true)
	{
		std::cout << "选择功能\n\
1:解析wix和wil文件. \n\
2:整理角色文件结构 \n\
3:整理武器文件结构 \n\
4:整理怪物文件结构0 \n\
5:整理怪物文件结构1 \n\
6:整理特效文件结构 \n\
7:将角色文件帧数据写入SQLite \n\
8:将武器文件帧数据写入SQLite \n\
9:将怪物文件帧数据写入SQLite \n\
10:将特效文件帧数据写入SQLite \n\
17:自动计算方向并分组 \n\
18:拆分位置文件 \n\
19:删除无效图片 \n\
20:按序号重命名文件 \n\
21:重命名为序列帧格式 \n\
22:提取地图资源\n\
23:分组图集\n\
24:打包图集\n\
25:打包全部图集\n\
26:转换地图文件\n\
27:转换全部地图文件\n\
28:解析所有wix和wil文件\n\
0:退出" << std::endl;
		int input;
		std::cin >> input;
		if (input == 0)
		{
			break;
		}
		else if (input == 1)
		{
			std::cout << "输入文件名:";
			std::string fileName;
			std::cin >> fileName;
			std::cout << "开始解析..." << std::endl;
			long startTime = timeGetTime();
			std::string filePath = "../media/" + fileName;
			std::string outputPath = "../media/" + fileName + "/";
			std::string WIX_SUFFIX = ".wix";
			std::string WIL_SUFFIX = ".wil";
			ImageUtility::wixWilToPNG(filePath + WIX_SUFFIX, filePath + WIL_SUFFIX, outputPath);
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		else if (input == 2)
		{
			std::cout << "输入文件夹名:";
			std::string fileName;
			std::cin >> fileName;
			std::cout << "开始整理角色文件结构..." << std::endl;
			long startTime = timeGetTime();
			std::string filePath = "../media/" + fileName;
			ImageUtility::autoGroupHumanImage(filePath);
			std::cout << "文件整理完毕,建议先给每个外观手动命名后再将帧数据写入SQLite" << std::endl;
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		else if (input == 3)
		{
			std::cout << "输入文件夹名:";
			std::string fileName;
			std::cin >> fileName;
			std::cout << "开始整理武器文件结构..." << std::endl;
			long startTime = timeGetTime();
			std::string filePath = "../media/" + fileName;
			ImageUtility::autoGroupWeaponImage(filePath);
			std::cout << "文件整理完毕,建议先给每个武器手动命名后再将帧数据写入SQLite" << std::endl;
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		else if (input == 4)
		{
			std::cout << "输入文件名:";
			std::string fileName;
			std::cin >> fileName;
			std::cout << "开始整理怪物文件结构0..." << std::endl;
			long startTime = timeGetTime();
			std::string filePath = "../media/" + fileName;
			ImageUtility::autoGroupMonsterImage0(filePath);
			std::cout << "请手动为每一个怪物进行动作分组,然后选择整理怪物文件结构1" << std::endl;
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		else if (input == 5)
		{
			std::cout << "输入文件名:";
			std::string fileName;
			std::cin >> fileName;
			std::cout << "开始整理怪物文件结构1..." << std::endl;
			long startTime = timeGetTime();
			std::string filePath = "../media/" + fileName;
			ImageUtility::autoGroupMonsterImage1(filePath);
			std::cout << "文件整理完毕,建议先给每个怪物手动命名后再将帧数据写入SQLite" << std::endl;
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		else if (input == 6)
		{
			std::cout << "输入文件夹名:";
			std::string fileName;
			std::cin >> fileName;
			std::cout << "开始整理特效文件结构..." << std::endl;
			long startTime = timeGetTime();
			std::string filePath = "../media/" + fileName;
			ImageUtility::autoGroupEffectImage(filePath);
			std::cout << "文件整理完毕,建议先给每个外观手动命名后再将帧数据写入SQLite" << std::endl;
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		else if (input == 7)
		{
			std::cout << "输入文件名:";
			std::string fileName;
			std::cin >> fileName;
			std::cout << "开始将角色文件帧数据写入SQLite..." << std::endl;
			long startTime = timeGetTime();
			std::string filePath = "../media/" + fileName;
			SQLite* mSQLite = TRACE_NEW(SQLite, mSQLite, "../media/DataBase.db");
			ImageUtility::saveFrameInfo(filePath, IT_HUMAN, mSQLite);
			TRACE_DELETE(mSQLite);
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		else if (input == 8)
		{
			std::cout << "输入文件名:";
			std::string fileName;
			std::cin >> fileName;
			std::cout << "开始将武器文件帧数据写入SQLite..." << std::endl;
			long startTime = timeGetTime();
			std::string filePath = "../media/" + fileName;
			SQLite* mSQLite = TRACE_NEW(SQLite, mSQLite, "../media/DataBase.db");
			ImageUtility::saveFrameInfo(filePath, IT_WEAPON, mSQLite);
			TRACE_DELETE(mSQLite);
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		else if (input == 9)
		{
			std::cout << "输入文件名:";
			std::string fileName;
			std::cin >> fileName;
			std::cout << "开始将怪物文件帧数据写入SQLite..." << std::endl;
			long startTime = timeGetTime();
			std::string filePath = "../media/" + fileName;
			SQLite* mSQLite = TRACE_NEW(SQLite, mSQLite, "../media/DataBase.db");
			ImageUtility::saveFrameInfo(filePath, IT_MONSTER, mSQLite);
			TRACE_DELETE(mSQLite);
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		else if (input == 10)
		{
			std::cout << "输入文件名:";
			std::string fileName;
			std::cin >> fileName;
			std::cout << "开始将特效文件帧数据写入SQLite..." << std::endl;
			long startTime = timeGetTime();
			std::string filePath = "../media/" + fileName;
			SQLite* mSQLite = TRACE_NEW(SQLite, mSQLite, "../media/DataBase.db");
			ImageUtility::saveFrameInfo(filePath, IT_EFFECT, mSQLite);
			TRACE_DELETE(mSQLite);
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		else if (input == 17)
		{
			std::cout << "输入文件夹名:";
			std::string fileName;
			std::cin >> fileName;
			std::cout << "开始自动计算方向并分组..." << std::endl;
			long startTime = timeGetTime();
			std::string filePath = "../media/" + fileName;
			ImageUtility::renameByDirection(filePath);
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		else if (input == 18)
		{
			std::cout << "输入文件夹名:";
			std::string fileName;
			std::cin >> fileName;
			std::cout << "开始拆分位置文件..." << std::endl;
			long startTime = timeGetTime();
			std::string filePath = "../media/" + fileName;
			ImageUtility::splitPositionFile(filePath);
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		else if (input == 19)
		{
			std::cout << "输入文件夹名:";
			std::string fileName;
			std::cin >> fileName;
			std::cout << "开始删除无效文件..." << std::endl;
			long startTime = timeGetTime();
			std::string filePath = "../media/" + fileName;
			ImageUtility::deleteInvalidImage(filePath);
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		else if (input == 20)
		{
			std::cout << "输入文件夹名:";
			std::string fileName;
			std::cin >> fileName;
			std::cout << "开始按序号重命名文件..." << std::endl;
			long startTime = timeGetTime();
			std::string filePath = "../media/" + fileName;
			ImageUtility::renameImage(filePath);
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		else if (input == 21)
		{
			std::cout << "输入文件夹名:";
			std::string fileName;
			std::cin >> fileName;
			std::cout << "开始重命名为序列帧格式..." << std::endl;
			long startTime = timeGetTime();
			std::string filePath = "../media/" + fileName;
			ImageUtility::renameImageToAnim(filePath);
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		else if (input == 22)
		{
			std::cout << "输入地图编号:";
			std::string fileName;
			std::cin >> fileName;
			std::cout << "开始提取地图资源..." << std::endl;
			long startTime = timeGetTime();
			std::string filePath = "../media/Map/" + fileName;
			ImageUtility::collectMapTexture(filePath);
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		else if (input == 23)
		{
			std::cout << "输入文件夹名:";
			std::string fileName;
			std::cin >> fileName;
			std::cout << "开始分组图集..." << std::endl;
			long startTime = timeGetTime();
			std::string filePath = "../media/" + fileName;
			ImageUtility::groupAtlas(filePath, 100);
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		else if (input == 24)
		{
			std::cout << "输入media下相对路径:";
			std::string fileName;
			std::cin >> fileName;
			std::cout << "开始打包图集..." << std::endl;
			long startTime = timeGetTime();
			std::string filePath = "../media/" + fileName;
			ImageUtility::texturePacker(filePath);
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		else if (input == 25)
		{
			std::cout << "输入media下相对路径:";
			std::string fileName;
			std::cin >> fileName;
			std::cout << "开始打包全部图集..." << std::endl;
			long startTime = timeGetTime();
			std::string filePath = "../media/" + fileName;
			ImageUtility::texturePackerAll(filePath);
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		else if (input == 26)
		{
			std::cout << "输入地图名:";
			std::string fileName;
			std::cin >> fileName;
			std::cout << "开始转换地图文件..." << std::endl;
			long startTime = timeGetTime();
			std::string filePath = "../media/Map/" + fileName;
			ImageUtility::convertMapFile(filePath);
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		else if (input == 27)
		{
			std::cout << "输入文件夹名:";
			std::string fileName;
			std::cin >> fileName;
			std::cout << "开始转换地图文件..." << std::endl;
			long startTime = timeGetTime();
			std::string filePath = "../media/" + fileName;
			ImageUtility::convertAllMapFile(filePath);
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		else if (input == 28)
		{
			std::cout << "开始解析所有wil..." << std::endl;
			long startTime = timeGetTime();
			ImageUtility::allWixWilToPNG("../media");
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		system("pause");
		std::cout << std::endl;
	}
	return;
}