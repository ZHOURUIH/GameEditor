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
		std::string fileName = fileList[i];
		std::string sourceFile = fileName;
		if (Utility::startWith(fileName, sourcePrePath))
		{
			sourceFile = fileName.substr(sourcePrePath.length(), fileName.length() - sourcePrePath.length());
		}
		std::string destFile = destPrePath + sourceFile;
		Utility::createFolder(Utility::getParentDir(destFile));
		if (!Utility::copyFile(fileName, destFile))
		{
			std::cout << "拷贝" << fileName << "失败" << std::endl;
			system("pause");
		}
		else
		{
			if (!Utility::deleteFile(fileName))
			{
				std::cout << "删除" << fileName << "失败" << std::endl;
				system("pause");
			}
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
	std::cin >> a;
	std::vector<std::string> folderList;
	std::string backupPath = GameConfig::getStringParam(GDS_COPY_FILE_DEST_DIR);
	if (a == 1)
	{
		std::string sourcePath = GameConfig::getStringParam(GDS_COPY_FILE_DIR);
		std::vector<std::string> backupFileList;
		// 查找需要备份的全部文件
		std::vector<std::string> pathList;
		Utility::split(sourcePath, ",", &pathList);
		int count = pathList.size();
		for (int i = 0; i < count; ++i)
		{
			Utility::findFiles(pathList[i].c_str(), backupFileList);
		}
		// 排除不备份的文件
		std::string exceptPath = GameConfig::getStringParam(GDS_EXCEPT_FILE);
		std::vector<std::string> exceptPathList;
		Utility::split(exceptPath, ",", &exceptPathList);
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
		std::vector<std::string> backupFileList;
		Utility::findFiles(backupPath.c_str(), backupFileList);
		moveResources(backupFileList, backupPath + "/", "../");
		deleteFolder(backupPath, folderList);
	}
	std::cout << "完成!" << std::endl;
	system("pause");
	return 0;
}