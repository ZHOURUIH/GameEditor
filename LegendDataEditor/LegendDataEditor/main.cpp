#include "ImageUtility.h"
#include "SQLiteData.h"

void main()
{
	// 各个工具函数的初始化需要优先调用
	MathUtility::initMathUtility();
	StringUtility::initStringUtility();
	SQLiteData::initFunctionList();

	while (true)
	{
		cout << "选择功能" << endl;
		cout << "2:自动计算角色,武器,翅膀图片名" << endl;
		cout << "4:整理怪物文件结构0" << endl;
		cout << "6:整理特效文件结构" << endl;
		cout << "7:自动计算方向" << endl;
		cout << "8:拆分位置文件" << endl;
		cout << "9:删除无效图片" << endl;
		cout << "10:按序号重命名文件,改为数字文件名" << endl;
		cout << "15:解析所有wix和wil文件" << endl;
		cout << "16:整理NPC文件结构" << endl;
		cout << "20:处理图片阴影" << endl;
		cout << "25:重新生成包含偏移量的图片" << endl;
		cout << "27:将图标调整到统一的36*36" << endl;
		cout << "32:将图片宽高缩放为原来的一半" << endl;
		cout << "35:将角色动作写入Animation表格" << endl;
		cout << "43:将怪物动作写入Animation表格" << endl;
		cout << endl;
		cout << "14:图集:打包全部图集" << endl;
		cout << "37:图集:图片划分到多个图集" << endl;
		cout << "38:图集:将所有目录的图片划分到多个图集" << endl;
		cout << endl;
		cout << "11:序列帧:重命名为序列帧格式" << endl;
		cout << "17:序列帧:将media中全部序列帧数据写入SQLite" << endl;
		cout << "26:序列帧:裁剪序列帧图片到最小尺寸" << endl;
		cout << "33:将图片的所有空白去除,并计算偏移量" << endl;
		cout << endl;
		cout << "22:地图:更新地图中的特效信息到SQLite" << endl;
		cout << "29:地图:转换所有地图文件为简化版本并且生成阻挡文件" << endl;
		cout << "34:检查所有地图是否有无效地砖下标" << endl;
		cout << "36:地图:输出地图解析文本" << endl;
		cout << "39:地图:生成地图地砖图片信息文件" << endl;
		cout << "40:地图:查看所有.map地图宽高" << endl;
		cout << "41:地图:将地图导出为txt格式" << endl;
		cout << "42:地图:通过txt格式生成map文件" << endl;
		cout << "0:退出" << endl;
		int input;
		cin >> input;
		if (input == 0)
		{
			break;
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
		else if (input == 4)
		{
			cout << "输入文件名:";
			string fileName;
			cin >> fileName;
			cout << "开始整理怪物文件结构..." << endl;
			long startTime = timeGetTime();
			ImageUtility::autoGroupMonsterImage("../media/" + fileName);
			cout << "文件整理完毕,建议先手动命名后,且确保每个文件夹中的图片打包后不会超出图集大小,再放入指定的目录中,然后再将帧数据写入SQLite" << endl;
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
			ImageUtility::renameImageToNumber("../media/" + fileName);
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
		else if (input == 14)
		{
			cout << "输入media下相对路径:";
			string fileName;
			cin >> fileName;
			cout << "开始打包全部图集..." << endl;
			long startTime = timeGetTime();
			ImageUtility::texturePackerAll("../media/" + fileName);
			cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		else if (input == 15)
		{
			cout << "开始解析所有wil..." << endl;
			long startTime = timeGetTime();
			ImageUtility::allWixWilToPNG("../media");
			cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		else if (input == 16)
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
		else if (input == 17)
		{
			cout << "开始将所有序列帧数据写入SQLite..." << endl;
			long startTime = timeGetTime();
			ImageUtility::writeAnimFrameSQLite();
			cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		else if (input == 20)
		{
			cout << "正在处理图片阴影,地图的图片请不要直接处理图集的阴影,否则会使其中单个图片的边缘受到影响..." << endl;
			long startTime = timeGetTime();
			ImageUtility::processAllShadow("../media/");
			cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		else if (input == 22)
		{
			cout << "正在更新地图中的特效信息到SQLite..." << endl;
			cout << "输入地图文件名, .map格式:";
			string fileName;
			cin >> fileName;
			cout << "输入地图的SceneMapFile表格ID:";
			int mapFileID;
			cin >> mapFileID;
			long startTime = timeGetTime();
			ImageUtility::writeMapEffectToSQLite(mapFileID, fileName);
			cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		else if (input == 25)
		{
			cout << "输入文件夹名:";
			string fileName;
			cin >> fileName;
			cout << "开始生成..." << endl;
			long startTime = timeGetTime();
			ImageUtility::generateAllOffsetedImage("../media/" + fileName);
			cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		else if (input == 26)
		{
			cout << "输入文件夹名:";
			string fileName;
			cin >> fileName;
			cout << "输入最低透明度:";
			int minAlpha;
			cin >> minAlpha;
			cout << "开始裁剪..." << endl;
			long startTime = timeGetTime();
			ImageUtility::trimAllImage("../media/" + fileName, minAlpha);
			cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		else if (input == 27)
		{
			cout << "输入文件夹名:";
			string fileName;
			cin >> fileName;
			cout << "开始调整图标大小..." << endl;
			long startTime = timeGetTime();
			ImageUtility::generateAllIconTo36("../media/" + fileName);
			cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		else if (input == 29)
		{
			cout << "开始转换地图以及生成阻挡文件..." << endl;
			long startTime = timeGetTime();
			ImageUtility::convertMapFile("../media");
			ImageUtility::generateAllUnreachFile("../media");
			cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		else if (input == 32)
		{
			cout << "输入文件夹名:";
			string fileName;
			cin >> fileName;
			ImageUtility::scaleTexture("../media/" + fileName, 0.5f);
		}
		else if (input == 33)
		{
			cout << "输入文件夹名:";
			string fileName;
			cin >> fileName;
			ImageUtility::fullImageToMinimal("../media/" + fileName);
		}
		else if (input == 34)
		{
			cout << "输入文件夹名:";
			string fileName;
			cin >> fileName;
			ImageUtility::checkMapTile("../media/" + fileName);
		}
		else if (input == 35)
		{
			cout << "输入角色中文名字:";
			string clothName;
			cin >> clothName;
			cout << "输入ImagePositionAnimation中的起始ID:";
			int startID;
			cin >> startID;
			ImageUtility::autoFillHumanAnimationTable(clothName, startID);
		}
		else if (input == 36)
		{
			cout << "输入地图文件名,带路径:";
			string fileName;
			cin >> fileName;
			ImageUtility::dumpMapFile("../media/" + fileName);
		}
		else if (input == 37)
		{
			cout << "输入文件夹名:";
			string fileName;
			cin >> fileName;
			ImageUtility::makeAtlasGroup("../media/" + fileName);
		}
		else if (input == 38)
		{
			const long startTime = timeGetTime();
			ImageUtility::makeAtlasGroupAll();
			cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		else if (input == 39)
		{
			cout << "输入地图图片目录,生成之前请先按图集划分好文件夹:";
			string fileName;
			cin >> fileName;
			ImageUtility::generateAtlasInfoFile("../media/" + fileName);
		}
		else if (input == 40)
		{
			ImageUtility::printMapSize("../media");
		}
		else if (input == 41)
		{
			cout << "输入地图文件名,文件为.map格式:";
			string fileName;
			cin >> fileName;
			ImageUtility::mapFileToTxt("../media/" + fileName);
		}
		else if (input == 42)
		{
			cout << "输入地图文本文件名,文件为.txt格式:";
			string fileName;
			cin >> fileName;
			ImageUtility::txtToMapFile("../media/" + fileName);
		}
		else if (input == 43)
		{
			cout << "输入怪物中文名字:";
			string monsterName;
			cin >> monsterName;
			cout << "输入ImagePositionAnimation中的起始ID:";
			int startID;
			cin >> startID;
			ImageUtility::autoFillMonsterAnimationTable(monsterName, startID);
		}
		system("pause");
		cout << endl;
	}
	return;
}