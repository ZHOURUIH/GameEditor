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
			if (ignoreFiles[i] == files[j])
			{
				files.erase(files.begin() + j);
				break;
			}
		}
	}
}

// 移动fileList中的所有文件,将列表中的所有文件从sourcePrePath移动到destPrePath中
void moveResources(const std::vector<std::string>& fileList, const std::string& sourcePrePath, const std::string& destPrePath)
{
	int fileCount = fileList.size();
	for (int i = 0; i < fileCount; ++i)
	{
		const std::string& fileName = fileList[i];
		std::string sourceFile;
		if (Utility::startWith(fileName, sourcePrePath))
		{
			sourceFile = fileName.substr(sourcePrePath.length(), fileName.length() - sourcePrePath.length());
		}
		else
		{
			sourceFile = fileName;
		}
		std::string destFile = destPrePath + sourceFile;
		Utility::createFolder(Utility::getParentDir(destFile));
		if (!Utility::moveFile(fileName, destFile))
		{
			std::cout << "移动" << fileName << "失败" << std::endl;
			system("pause");
		}
	}
}

// 删除空文件夹
void deleteFolder(std::string path, std::vector<std::string> folderList)
{
	Utility::deleteEmptyFolder(path, folderList);
	int count = (int)folderList.size();
	for (int i = 0; i < count; ++i)
	{
		Utility::deleteFolder(folderList[i], true);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	TRACE_NEW(GameFramework, mGameFramework);
	mGameFramework->init();
	std::cout << "1.备份资源" << std::endl;
	std::cout << "2.恢复资源" << std::endl;
	int a = 0;
	std::string backupPath = GameConfig::getStringParam(GDS_COPY_FILE_DEST_DIR);
	// 目标目录存在且有文件,则需要询问是否恢复
	if (!Utility::isFolderEmpty(backupPath))
	{
		std::cout << "备份目标目录不为空,是否想要恢复文件?(y(恢复文件)/n(继续备份文件))";
		char input = 0;
		std::cin >> input;
		if (input == 'y')
		{
			a = 2;
		}
		else
		{
			a = 1;
		}
	}
	else
	{
		std::cout << "请选择:";
		std::cin >> a;
	}
	long startTime = timeGetTime();
	std::vector<std::string> folderList;
	if (a == 1)
	{
		std::cout << "正在备份..." << std::endl;
		std::string sourcePath = GameConfig::getStringParam(GDS_COPY_FILE_DIR);
		std::vector<std::string> backupFileList;
		// 查找需要备份的全部文件
		std::vector<std::string> pathList;
		Utility::split(sourcePath, ",", pathList);
		int count = pathList.size();
		for (int i = 0; i < count; ++i)
		{
			Utility::findFiles(pathList[i].c_str(), backupFileList);
		}
		// 排除不备份的文件
		std::string exceptPath = GameConfig::getStringParam(GDS_EXCEPT_FILE);
		std::vector<std::string> exceptPathList;
		Utility::split(exceptPath, ",", exceptPathList);
		int exceptPathCount = (int)exceptPathList.size();
		for (int i = 0; i < (int)backupFileList.size();)
		{
			bool isExceptFile = false;
			for (int j = 0; j < exceptPathCount; ++j)
			{
				if (Utility::startWith(backupFileList[i], exceptPathList[j]))
				{
					isExceptFile = true;
					break;
				}
			}
			if (isExceptFile)
			{
				backupFileList.erase(backupFileList.begin() + i);
			}
			else
			{
				++i;
			}
		}
		// 拷贝文件到备份文件夹
		Utility::deleteFolder(backupPath, false);
		moveResources(backupFileList, "../", backupPath + "/");
		for (int i = 0; i < count; ++i)
		{
			deleteFolder(pathList[i], folderList);
		}
	}
	else
	{
		std::cout << "正在恢复..." << std::endl;
		std::vector<std::string> backupFileList;
		Utility::findFiles(backupPath.c_str(), backupFileList);
		moveResources(backupFileList, backupPath + "/", "../");
		deleteFolder(backupPath, folderList);
	}
	long deltaTime = timeGetTime() - startTime;
	std::cout << "完成! 耗时 : " << deltaTime / 1000.0f << "秒" << std::endl;
	system("pause");
	return 0;
}