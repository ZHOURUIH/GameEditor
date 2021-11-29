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
		cout << "1:解析wix和wil文件" << endl;
		cout << "2:整理角色文件结构" << endl;
		cout << "3:整理武器文件结构" << endl;
		cout << "4:整理怪物文件结构0" << endl;
		cout << "5:整理怪物文件结构1" << endl;
		cout << "6:整理特效文件结构" << endl;
		cout << "7:自动计算方向" << endl;
		cout << "8:拆分位置文件" << endl;
		cout << "9:删除无效图片" << endl;
		cout << "10:按序号重命名文件" << endl;
		cout << "11:重命名为序列帧格式" << endl;
		cout << "13:分组图集" << endl;
		cout << "14:打包图集" << endl;
		cout << "15:打包全部图集" << endl;
		cout << "18:解析所有wix和wil文件" << endl;
		cout << "19:整理NPC文件结构" << endl;
		cout << "20:将media中全部序列帧数据写入SQLite" << endl;
		cout << "22:生成地图阻挡文件" << endl;
		cout << "23:生成所有地图阻挡文件" << endl;
		cout << "25:处理图片阴影" << endl;
		cout << "26:移动并覆盖地图图片" << endl;
		cout << "27:更新NPC商品列表" << endl;
		cout << "28:更新地图中的特效信息到SQLite" << endl;
		cout << "29:更新Animation表格中的AnimationPosition" << endl;
		cout << "30:整理翅膀文件结构" << endl;
		cout << "31:重新生成包含偏移量的图片" << endl;
		cout << "32:裁剪序列帧图片到最小尺寸" << endl;
		cout << "33:将图标调整到统一的最大尺寸" << endl;
		cout << "0:退出" << endl;
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
		else if (input == 25)
		{
			cout << "正在处理图片阴影..." << endl;
			long startTime = timeGetTime();
			ImageUtility::processAllShadow("../media/");
			cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		else if (input == 26)
		{
			cout << "输入路径:";
			string fileName;
			cin >> fileName;
			cout << "正在移动图片..." << endl;
			long startTime = timeGetTime();
			ImageUtility::moveMapObjectTexture(fileName);
			cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		else if (input == 27)
		{
			cout << "正在更新NPC商品列表..." << endl;
			long startTime = timeGetTime();
			ImageUtility::updateNPCGoodsSQLite();
			cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		else if (input == 28)
		{
			cout << "正在更新地图中的特效信息到SQLite..." << endl;
			long startTime = timeGetTime();
			ImageUtility::updateMapEffect();
			cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		else if (input == 29)
		{
			cout << "正在更新Animation表格中的AnimationPosition..." << endl;
			long startTime = timeGetTime();
			ImageUtility::updateAnimationPositionInAnimation();
			cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		else if (input == 30)
		{
			cout << "输入文件夹名:";
			string fileName;
			cin >> fileName;
			cout << "开始整理翅膀文件结构..." << endl;
			long startTime = timeGetTime();
			ImageUtility::autoGroupWingImage("../media/" + fileName);
			cout << "文件整理完毕,建议先给每个外观手动命名后再将帧数据写入SQLite" << endl;
			cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		else if (input == 31)
		{
			cout << "输入文件夹名:";
			string fileName;
			cin >> fileName;
			cout << "开始生成..." << endl;
			long startTime = timeGetTime();
			ImageUtility::generateAllOffsetedImage("../media/" + fileName);
			cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		else if (input == 32)
		{
			cout << "输入文件夹名:";
			string fileName;
			cin >> fileName;
			cout << "开始裁剪..." << endl;
			long startTime = timeGetTime();
			ImageUtility::trimAllImage("../media/" + fileName);
			cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		else if (input == 33)
		{
			cout << "输入文件夹名:";
			string fileName;
			cin >> fileName;
			cout << "开始调整图标大小..." << endl;
			long startTime = timeGetTime();
			ImageUtility::generateAllIconToMaxSize("../media/" + fileName);
			cout << "耗时 : " << (timeGetTime() - startTime) / 1000.0f << "秒" << endl;
		}
		system("pause");
		cout << endl;
	}
	return;
}