#include <cstdio>
#include <cstdlib>
#include <direct.h>
#include <string>
#include <vector>
#include <windows.h>
#include <iostream>

#include "Utility.h"

void adjustFileName(std::string& fileName)
{
	std::string preStr = fileName.substr(0, fileName.find_last_of('_') + 1);
	std::string lastStr = fileName.substr(preStr.length(), fileName.find_last_of('.') - preStr.length());
	std::string suffix = fileName.substr(fileName.find_last_of('.'), fileName.length() - fileName.find_last_of('.'));
	fileName = preStr + Utility::intToString(Utility::stringToInt(lastStr), 2) + suffix;
}

void recoverFileName(std::string& fileName)
{
	std::string preStr = fileName.substr(0, fileName.find_last_of('_') + 1);
	std::string lastStr = fileName.substr(preStr.length(), fileName.find_last_of('.') - preStr.length());
	std::string suffix = fileName.substr(fileName.find_last_of('.'), fileName.length() - fileName.find_last_of('.'));
	fileName = preStr + Utility::intToString(Utility::stringToInt(lastStr)) + suffix;
}

void main()
{
	std::vector<std::string> fileList;
	Utility::findFiles(".", fileList, ".png");
	int fileCount = fileList.size();
	if (fileCount == 0)
	{
		return;
	}
	Vector2 size;
	for (int i = 0; i < fileCount; ++i)
	{
		Vector2 curSize = Utility::getImageSize(fileList[i]);
		if (size.x != 0 && size.y != 0)
		{
			if (curSize.x != size.x || curSize.y != size.y)
			{
				std::cout << "已经裁剪过的序列帧不能再次被裁剪!" << std::endl;
				//system("pause");
				//return;
			}
		}
		else
		{
			size = curSize;
		}
	}
	for (int i = 0; i < fileCount; ++i)
	{
		adjustFileName(fileList[i]);
	}
	std::sort(fileList.begin(), fileList.begin() + fileCount);
	for (int i = 0; i < fileCount; ++i)
	{
		recoverFileName(fileList[i]);
	}
	Vector2 targetSize;
	std::cout << "请输入序列帧目标宽:";
	std::cin >> targetSize.x;
	std::cout << "请输入序列帧目标高:";
	std::cin >> targetSize.y;
	std::string fileContent = "TargetSize=" + Utility::intToString(targetSize.x) + " * " + Utility::intToString(targetSize.y) + "\r\n";
	for (int i = 0; i < fileCount; ++i)
	{
		Vector2 imageNewSize, imageTargetSize;
		FloatVector2 imageTargetPos;
		Utility::convert(fileList[i], targetSize, imageNewSize, imageTargetSize, imageTargetPos);
		fileContent += Utility::getFileNameNoSuffix(fileList[i]) + " = TargetSize:" + Utility::intToString(imageTargetSize.x) + "*" + Utility::intToString(imageTargetSize.y) + "; ";
		fileContent += "Pos:" + Utility::floatToString(imageTargetPos.x) + "," + Utility::floatToString(imageTargetPos.y) + "; ";
		fileContent += "OriginSize:" + Utility::intToString(imageNewSize.x) + " * " + Utility::intToString(imageNewSize.y) + ";\r\n";
	}
	fileContent = Utility::ANSIToUTF8(fileContent, true);
	std::string animName = Utility::getFileNameNoSuffix(fileList[0]);
	animName = animName.substr(0, animName.find_last_of('_'));
	Utility::writeFile(Utility::getParentDir(fileList[0]) + "/" + animName + ".txt", fileContent.length(), fileContent.c_str());
}