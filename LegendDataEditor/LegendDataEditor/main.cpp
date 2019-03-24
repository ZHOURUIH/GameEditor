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
7:自动计算方向 \n\
8:拆分位置文件 \n\
9:删除无效图片 \n\
10:按序号重命名文件 \n\
11:重命名为序列帧格式 \n\
12:提取地图资源\n\
13:分组图集\n\
14:打包图集\n\
15:打包全部图集\n\
16:转换地图文件\n\
17:转换全部地图文件\n\
18:解析所有wix和wil文件\n\
19:整理NPC文件结构\n\
20:将media中全部序列帧数据写入SQLite \n\
21:将图片位置偏移写入SQLite \n\
22:将怪物碰撞范围写入SQLite \n\
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
			string fileName;
			std::cin >> fileName;
			std::cout << "开始解析..." << std::endl;
			long startTime = timeGetTime();
			string filePath = "../media/" + fileName;
			string outputPath = "../media/" + fileName + "/";
			ImageUtility::wixWilToPNG(filePath + ".wix", filePath + ".wil", outputPath);
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		else if (input == 2)
		{
			std::cout << "输入文件夹名:";
			string fileName;
			std::cin >> fileName;
			std::cout << "开始整理角色文件结构..." << std::endl;
			long startTime = timeGetTime();
			ImageUtility::autoGroupHumanImage("../media/" + fileName);
			std::cout << "文件整理完毕,建议先给每个外观手动命名后再将帧数据写入SQLite" << std::endl;
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		else if (input == 3)
		{
			std::cout << "输入文件夹名:";
			string fileName;
			std::cin >> fileName;
			std::cout << "开始整理武器文件结构..." << std::endl;
			long startTime = timeGetTime();
			ImageUtility::autoGroupWeaponImage("../media/" + fileName);
			std::cout << "文件整理完毕,建议先给每个武器手动命名后再将帧数据写入SQLite" << std::endl;
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		else if (input == 4)
		{
			std::cout << "输入文件名:";
			string fileName;
			std::cin >> fileName;
			std::cout << "开始整理怪物文件结构0..." << std::endl;
			long startTime = timeGetTime();
			ImageUtility::autoGroupMonsterImage0("../media/" + fileName);
			std::cout << "请手动为每一个怪物进行动作分组,然后选择整理怪物文件结构1" << std::endl;
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		else if (input == 5)
		{
			std::cout << "输入文件名:";
			string fileName;
			std::cin >> fileName;
			std::cout << "开始整理怪物文件结构1..." << std::endl;
			long startTime = timeGetTime();
			ImageUtility::autoGroupMonsterImage1("../media/" + fileName);
			std::cout << "文件整理完毕,建议先给每个怪物手动命名后再将帧数据写入SQLite" << std::endl;
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		else if (input == 6)
		{
			std::cout << "输入文件夹名:";
			string fileName;
			std::cin >> fileName;
			std::cout << "开始整理特效文件结构..." << std::endl;
			long startTime = timeGetTime();
			ImageUtility::autoGroupEffectImage("../media/" + fileName);
			std::cout << "文件整理完毕,建议先给每个外观手动命名后再将帧数据写入SQLite" << std::endl;
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		else if (input == 7)
		{
			std::cout << "输入文件夹名:";
			string fileName;
			std::cin >> fileName;
			std::cout << "输入方向数量:";
			int directionCount = 1;
			std::cin >> directionCount;
			std::cout << "开始自动计算方向..." << std::endl;
			long startTime = timeGetTime();
			ImageUtility::renameByDirection("../media/" + fileName, directionCount, false);
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		else if (input == 8)
		{
			std::cout << "输入文件夹名:";
			string fileName;
			std::cin >> fileName;
			std::cout << "开始拆分位置文件..." << std::endl;
			long startTime = timeGetTime();
			ImageUtility::splitPositionFile("../media/" + fileName);
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		else if (input == 9)
		{
			std::cout << "输入文件夹名:";
			string fileName;
			std::cin >> fileName;
			std::cout << "开始删除无效文件..." << std::endl;
			long startTime = timeGetTime();
			ImageUtility::deleteInvalidImage("../media/" + fileName);
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		else if (input == 10)
		{
			std::cout << "输入文件夹名:";
			string fileName;
			std::cin >> fileName;
			std::cout << "开始按序号重命名文件..." << std::endl;
			long startTime = timeGetTime();
			ImageUtility::renameImage("../media/" + fileName);
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		else if (input == 11)
		{
			std::cout << "输入文件夹名:";
			string fileName;
			std::cin >> fileName;
			std::cout << "开始重命名为序列帧格式..." << std::endl;
			long startTime = timeGetTime();
			ImageUtility::renameImageToAnim("../media/" + fileName);
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		else if (input == 12)
		{
			std::cout << "输入地图编号:";
			string fileName;
			std::cin >> fileName;
			std::cout << "开始提取地图资源..." << std::endl;
			long startTime = timeGetTime();
			ImageUtility::collectMapTexture("../media/Map/" + fileName);
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		else if (input == 13)
		{
			std::cout << "输入文件夹名:";
			string fileName;
			std::cin >> fileName;
			std::cout << "开始分组图集..." << std::endl;
			long startTime = timeGetTime();
			ImageUtility::groupAtlas("../media/" + fileName, 100);
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		else if (input == 14)
		{
			std::cout << "输入media下相对路径:";
			string fileName;
			std::cin >> fileName;
			std::cout << "开始打包图集..." << std::endl;
			long startTime = timeGetTime();
			ImageUtility::texturePacker("../media/" + fileName);
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		else if (input == 15)
		{
			std::cout << "输入media下相对路径:";
			string fileName;
			std::cin >> fileName;
			std::cout << "开始打包全部图集..." << std::endl;
			long startTime = timeGetTime();
			ImageUtility::texturePackerAll("../media/" + fileName);
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		else if (input == 16)
		{
			std::cout << "输入地图名:";
			string fileName;
			std::cin >> fileName;
			std::cout << "开始转换地图文件..." << std::endl;
			long startTime = timeGetTime();
			ImageUtility::convertMapFile("../media/Map/" + fileName);
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		else if (input == 17)
		{
			std::cout << "输入文件夹名:";
			string fileName;
			std::cin >> fileName;
			std::cout << "开始转换地图文件..." << std::endl;
			long startTime = timeGetTime();
			ImageUtility::convertAllMapFile("../media/" + fileName);
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		else if (input == 18)
		{
			std::cout << "开始解析所有wil..." << std::endl;
			long startTime = timeGetTime();
			ImageUtility::allWixWilToPNG("../media");
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		else if (input == 19)
		{
			std::cout << "输入文件夹名:";
			string fileName;
			std::cin >> fileName;
			std::cout << "开始整理NPC文件结构..." << std::endl;
			long startTime = timeGetTime();
			ImageUtility::autoGroupNPCImage("../media/" + fileName);
			std::cout << "文件整理完毕" << std::endl;
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		else if (input == 20)
		{
			std::cout << "开始将所有序列帧数据写入SQLite..." << std::endl;
			char updateOnly = 'y';
			std::cout << "是否只更新数据? y(是)/n(否)";
			std::cin >> updateOnly;
			long startTime = timeGetTime();
			if (updateOnly == 'y' || updateOnly == 'n')
			{
				ImageUtility::writeAnimFrameSQLite(updateOnly == 'y');
			}
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		else if (input == 21)
		{
			std::cout << "输入media下相对路径:";
			string fileName;
			std::cin >> fileName;
			std::cout << "开始将图片位置偏移写入SQLite..." << std::endl;
			long startTime = timeGetTime();
			ImageUtility::writeImagePosSQLite("../media/" + fileName);
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		else if (input == 22)
		{
			std::cout << "开始将怪物碰撞范围数据写入SQLite..." << std::endl;
			char updateOnly = 'y';
			std::cout << "是否只更新数据? y(是)/n(否)";
			std::cin >> updateOnly;
			long startTime = timeGetTime();
			if (updateOnly == 'y' || updateOnly == 'n')
			{
				ImageUtility::writeMonsterColliderSQLite(updateOnly == 'y');
			}
			std::cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << std::endl;
		}
		system("pause");
		std::cout << std::endl;
	}
	return;
}