// ReleaseGame.cpp : 定义控制台应用程序的入口点。
//

#include <stdio.h>
#include <tchar.h>
#include <vector>

#include <windows.h>      // Header File For Windows
#include <windowsx.h>
#include <mmsystem.h>

#include "Utility.h"
#include "CommonDefine.h"
#include "GameConfig.h"
#include "GameFramework.h"
#include "txMemeryTrace.h"

GameFramework* mGameFramework = NULL;

void removeIgnoreFile(const std::vector<std::string>& ignoreFiles, std::vector<std::string>& files)
{
	int ignoreFileCount = ignoreFiles.size();
	for (int i = 0; i < ignoreFileCount; ++i)
	{
		int allFileCount = files.size();
		for (int j = 0; j < allFileCount; ++j)
		{
			if (ignoreFiles[i] == Utility::getFileName(files[j]))
			{
				files.erase(files.begin() + j);
				break;
			}
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	 TRACE_NEW(GameFramework, mGameFramework);
	mGameFramework->init();

	// 读取配置文件，获得需要加载的所有数据列表
	// 遍历每一个文件名，加载相应的文件

	// 需要忽略的文件
	std::string ignoreFile = GameConfig::getStringParam(GDS_EXCEPT_FILE);
	std::vector<std::string> ignoreFiles;
	Utility::split(ignoreFile, ",", &ignoreFiles);

	// 拷贝exe
	std::vector<std::string> fileList;
	// 源目录
	std::string strPathName = GameConfig::getStringParam(GDS_COPY_RESOUCE_DIR);
	std::vector<std::string> patterns;
	patterns.push_back(".dll");
	patterns.push_back(".exe");
	patterns.push_back(".pdb");
	Utility::findFiles(strPathName.c_str(), fileList, patterns);
	// 去掉所有忽略的文件
	removeIgnoreFile(ignoreFiles, fileList);
	// 游戏名
	std::string gameName = GameConfig::getStringParam(GDS_COPY_DEST_GAME_NAME);
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	char timeName[1024];
	sprintf_s(timeName, 1024, "_%d-%d-%d", sys.wYear, sys.wMonth, sys.wDay);

	// 目标目录
	std::string destPath = gameName + timeName + "/" + GameConfig::getStringParam(GDS_COPY_DEST_DIR);
	Utility::createFolder(destPath);

	// bin目录的所有文件
	int listSize = fileList.size();
	for (int i = 0; i < listSize; ++i)
	{
		std::string destName;
		int splitPos = fileList[i].find_first_of(strPathName);
		int lastPos = fileList[i].find_last_of("/");
		std::string folderName = destPath + fileList[i].substr(splitPos + strPathName.length(), lastPos - splitPos - strPathName.length());
		Utility::createFolder(folderName);
		
		if (!Utility::endWith(fileList[i], "exe"))
		{
			destName = folderName + "/" + Utility::getFileName(fileList[i]);
		}
		else
		{
			destName = folderName + "/" + GameConfig::getStringParam(GDS_COPY_DEST_NAME) + ".exe";
		}
		Utility::copyFile(fileList[i], destName, true);
	}

	// 拷贝media
	std::vector<std::string> mediaFileList;
	// 源目录
	std::string mediaPathName = GameConfig::getStringParam(GDS_COPY_RESOUCE_MEDIA_DIR); 
	std::vector<std::string> mediaPatterns;
	Utility::findFiles(mediaPathName.c_str(), mediaFileList, mediaPatterns);
	// 去掉所有忽略的文件
	removeIgnoreFile(ignoreFiles, mediaFileList);
	// 目标目录
	std::string mediaDestPath = gameName + timeName + "/" +GameConfig::getStringParam(GDS_COPY_DEST_MEDIA_DIR);
	Utility::createFolder(mediaDestPath);
	int mediaListSize = mediaFileList.size();
	for (int i = 0; i < mediaListSize; ++i)
	{
		std::string destName;
		int splitPos = mediaFileList[i].find_first_of(mediaPathName);
		int lastPos = mediaFileList[i].find_last_of("/");
		std::string folderName = mediaDestPath + mediaFileList[i].substr(splitPos + mediaPathName.length(), lastPos - splitPos - mediaPathName.length());
		Utility::createFolder(folderName);
		destName = folderName + "/" + Utility::getFileName(mediaFileList[i]);
		Utility::copyFile(mediaFileList[i], destName, true);
	}
	std::cout << "版本已生成完成!" << std::endl;
	system("pause");
	return 0;
}