#include <iostream>
#include "Utility.h"
#include "SQLite.h"

using namespace std;

void main()
{
	while (true)
	{
		cout << "选择功能\n\
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
13:分组图集\n\
14:打包图集\n\
15:打包全部图集\n\
18:解析所有wix和wil文件\n\
19:整理NPC文件结构\n\
20:将media中全部序列帧数据写入SQLite \n\
21:打包地图图集 \n\
22:生成地图阻挡文件 \n\
23:生成所有地图阻挡文件 \n\
24:更新场景传送点列表 \n\
25:更新场景NPC列表 \n\
26:更新场景刷怪区域列表 \n\
27:更新场景安全区列表 \n\
28:处理图片阴影\n\
0:退出" << endl;
		int input;
		cin >> input;
		if (input == 0)
		{
			break;
		}
		else if (input == 1)
		{
			cout << "输入文件名:";
			string fileName;
			cin >> fileName;
			cout << "开始解析..." << endl;
			long startTime = timeGetTime();
			string filePath = "../media/" + fileName;
			string outputPath = "../media/" + fileName + "/";
			ImageUtility::wixWilToPNG(filePath + ".wix", filePath + ".wil", outputPath);
			cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		else if (input == 2)
		{
			cout << "输入文件夹名:";
			string fileName;
			cin >> fileName;
			cout << "开始整理角色文件结构..." << endl;
			long startTime = timeGetTime();
			ImageUtility::autoGroupHumanImage("../media/" + fileName);
			cout << "文件整理完毕,建议先给每个外观手动命名后再将帧数据写入SQLite" << endl;
			cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		else if (input == 3)
		{
			cout << "输入文件夹名:";
			string fileName;
			cin >> fileName;
			cout << "开始整理武器文件结构..." << endl;
			long startTime = timeGetTime();
			ImageUtility::autoGroupWeaponImage("../media/" + fileName);
			cout << "文件整理完毕,建议先给每个武器手动命名后再将帧数据写入SQLite" << endl;
			cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		else if (input == 4)
		{
			cout << "输入文件名:";
			string fileName;
			cin >> fileName;
			cout << "开始整理怪物文件结构0..." << endl;
			long startTime = timeGetTime();
			ImageUtility::autoGroupMonsterImage0("../media/" + fileName);
			cout << "请手动为每一个怪物进行动作分组,然后选择整理怪物文件结构1" << endl;
			cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		else if (input == 5)
		{
			cout << "输入文件名:";
			string fileName;
			cin >> fileName;
			cout << "开始整理怪物文件结构1..." << endl;
			long startTime = timeGetTime();
			ImageUtility::autoGroupMonsterImage1("../media/" + fileName);
			cout << "文件整理完毕,建议先给每个怪物手动命名后再将帧数据写入SQLite" << endl;
			cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		else if (input == 6)
		{
			cout << "输入文件夹名:";
			string fileName;
			cin >> fileName;
			cout << "开始整理特效文件结构..." << endl;
			long startTime = timeGetTime();
			ImageUtility::autoGroupEffectImage("../media/" + fileName);
			cout << "文件整理完毕,建议先给每个外观手动命名后再将帧数据写入SQLite" << endl;
			cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		else if (input == 7)
		{
			cout << "输入文件夹名:";
			string fileName;
			cin >> fileName;
			cout << "输入方向数量:";
			int directionCount = 1;
			cin >> directionCount;
			cout << "开始自动计算方向..." << endl;
			long startTime = timeGetTime();
			ImageUtility::renameByDirection("../media/" + fileName, directionCount, false);
			cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		else if (input == 8)
		{
			cout << "输入文件夹名:";
			string fileName;
			cin >> fileName;
			cout << "开始拆分位置文件..." << endl;
			long startTime = timeGetTime();
			ImageUtility::splitPositionFile("../media/" + fileName);
			cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		else if (input == 9)
		{
			cout << "输入文件夹名:";
			string fileName;
			cin >> fileName;
			cout << "开始删除无效文件..." << endl;
			long startTime = timeGetTime();
			ImageUtility::deleteInvalidImage("../media/" + fileName);
			cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		else if (input == 10)
		{
			cout << "输入文件夹名:";
			string fileName;
			cin >> fileName;
			cout << "开始按序号重命名文件..." << endl;
			long startTime = timeGetTime();
			ImageUtility::renameImage("../media/" + fileName);
			cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		else if (input == 11)
		{
			cout << "输入文件夹名:";
			string fileName;
			cin >> fileName;
			cout << "开始重命名为序列帧格式..." << endl;
			long startTime = timeGetTime();
			ImageUtility::renameImageToAnim("../media/" + fileName);
			cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		else if (input == 13)
		{
			cout << "输入文件夹名:";
			string fileName;
			cin >> fileName;
			cout << "开始分组图集..." << endl;
			long startTime = timeGetTime();
			ImageUtility::groupAtlas("../media/" + fileName, 100);
			cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		else if (input == 14)
		{
			cout << "输入media下相对路径:";
			string fileName;
			cin >> fileName;
			cout << "开始打包图集..." << endl;
			long startTime = timeGetTime();
			ImageUtility::texturePacker("../media/" + fileName);
			cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		else if (input == 15)
		{
			cout << "输入media下相对路径:";
			string fileName;
			cin >> fileName;
			cout << "开始打包全部图集..." << endl;
			long startTime = timeGetTime();
			ImageUtility::texturePackerAll("../media/" + fileName);
			cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		else if (input == 18)
		{
			cout << "开始解析所有wil..." << endl;
			long startTime = timeGetTime();
			ImageUtility::allWixWilToPNG("../media");
			cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		else if (input == 19)
		{
			cout << "输入文件夹名:";
			string fileName;
			cin >> fileName;
			cout << "开始整理NPC文件结构..." << endl;
			long startTime = timeGetTime();
			ImageUtility::autoGroupNPCImage("../media/" + fileName);
			cout << "文件整理完毕" << endl;
			cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		else if (input == 20)
		{
			cout << "开始将所有序列帧数据写入SQLite..." << endl;
			char updateOnly = 'y';
			cout << "是否只更新数据? y(是)/n(否)";
			cin >> updateOnly;
			long startTime = timeGetTime();
			if (updateOnly == 'y' || updateOnly == 'n')
			{
				ImageUtility::writeAnimFrameSQLite(updateOnly == 'y');
			}
			cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		else if (input == 21)
		{
			cout << "输入media下相对路径:";
			string fileName;
			cin >> fileName;
			cout << "开始打包地图图集..." << endl;
			long startTime = timeGetTime();
			ImageUtility::packMapTextureAll("../media/" + fileName);
			cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		else if (input == 22)
		{
			cout << "输入media下相对路径:";
			string fileName;
			cin >> fileName;
			cout << "正在生成阻挡文件..." << endl;
			long startTime = timeGetTime();
			ImageUtility::generateUnreachFile("../media/" + fileName);
			cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		else if (input == 23)
		{
			cout << "正在生成阻挡文件..." << endl;
			long startTime = timeGetTime();
			ImageUtility::generateAllUnreachFile("../media");
			cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		else if (input == 24)
		{
			cout << "正在更新场景传送点列表..." << endl;
			long startTime = timeGetTime();
			ImageUtility::updateSceneMapTransferSQLite();
			cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		else if (input == 25)
		{
			cout << "正在更新场景NPC列表..." << endl;
			long startTime = timeGetTime();
			ImageUtility::updateSceneMapNPCSQLite();
			cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		else if (input == 26)
		{
			cout << "正在更新场景刷怪区域列表..." << endl;
			long startTime = timeGetTime();
			ImageUtility::updateSceneMapMonsterRegionSQLite();
			cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		else if (input == 27)
		{
		cout << "正在更新场景安全区列表..." << endl;
		long startTime = timeGetTime();
		ImageUtility::updateSceneMapPeaceAreaSQLite();
		cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		else if (input == 28)
		{
			cout << "正在处理图片阴影..." << endl;
			long startTime = timeGetTime();
			ImageUtility::processAllShadow("../media/");
			cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		system("pause");
		cout << endl;
	}
	return;
}