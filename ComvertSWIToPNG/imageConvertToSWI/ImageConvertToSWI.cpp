#include <cstdio>
#include <cstdlib>
#include <direct.h>
#include <string>
#include <vector>
#include <windows.h>
#include <iostream>
#include <io.h>

#include "FreeImage.h"

void findFiles(std::string path, std::vector<std::string>& fileList, std::vector<std::string> patterns)
{
	WIN32_FIND_DATA FindData;
	HANDLE hError;
	// 构造路径
	std::string pathName = path + "/*.*";
	hError = FindFirstFile(pathName.c_str(), &FindData);
	if (hError == INVALID_HANDLE_VALUE)
	{
		return;
	}
	while (::FindNextFile(hError, &FindData))
	{
		// 过虑.和..
		if (strcmp(FindData.cFileName, ".") == 0
			|| strcmp(FindData.cFileName, "..") == 0)
		{
			continue;
		}

		// 构造完整路径
		std::string fullname = path + "/" + std::string(FindData.cFileName);
		if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			findFiles(fullname, fileList, patterns);
		}
		else
		{
			int patternCount = patterns.size();
			for (int i = 0; i < patternCount; ++i)
			{
				if (fullname.length() >= patterns[i].length() && fullname.substr(fullname.length() - patterns[i].length(), patterns[i].length()) == patterns[i])
				{
					fileList.push_back(fullname);
				}
			}
		}
	}
}

void writeBuffer(char* buffer, void* src, int writeSize, int bufferSize, int& offset)
{
	if (offset + writeSize > bufferSize)
	{
		return;
	}
	memcpy(buffer + offset, src, writeSize);
	offset += writeSize;
}

int convert(std::string src, std::string dst)
{
	FILE* pf = NULL;
	fopen_s(&pf, src.c_str(), "rb");
	if (pf == NULL)
	{
		return 0;
	}
	fseek(pf, 0, SEEK_END);
	int fileSize = ftell(pf);
	rewind(pf);
	char* fileBuffer = new char[fileSize];
	fread(fileBuffer, sizeof(char), fileSize, pf);
	fclose(pf);

	short width = *(short*)(fileBuffer + 4);
	short height = *(short*)(fileBuffer + 6);
	char* pixelData = fileBuffer + 8;

	FIBITMAP* bitmapRet = FreeImage_Allocate(width, height, 32);
	
	for (int y = 0; y < height; ++y)
	{
		BYTE* bits = FreeImage_GetScanLine(bitmapRet, y);
		for (int x = 0; x < width; ++x)
		{
			bits[x * 4 + 0] = pixelData[y * width * 4 + 4 * x + 2];
			bits[x * 4 + 1] = pixelData[y * width * 4 + 4 * x + 1];
			bits[x * 4 + 2] = pixelData[y * width * 4 + 4 * x + 0];
			bits[x * 4 + 3] = pixelData[y * width * 4 + 4 * x + 3];
		}
	}

	// 保存为png
	FreeImage_Save(FIF_PNG, bitmapRet, dst.c_str());
	FreeImage_Unload(bitmapRet);
	return 1;
}

int main(int argc, char **argv)
{
	std::vector<std::string> patterns;
	patterns.push_back(".swi");
	std::vector<std::string> fileList;
	findFiles(".", fileList, patterns);

	int fileCount = fileList.size();
	if (fileCount == 0)
	{
		return 0;
	}

	FreeImage_Initialise(1);
	bool hasError = false;
	for (int i = 0; i < fileCount; ++i)
	{
		std::string srcpath = fileList[i];
		std::string destPath = "./ret/" + srcpath;
		std::string destDir = destPath.substr(0, destPath.find_last_of('/'));
		if (_access(destDir.c_str(), 0) == -1)
		{
			std::cout << "无法找到路径:" << destDir.c_str() << std::endl;
			hasError = true;
			
			continue;
		}
		std::string noSuffix = destPath.substr(0, destPath.find_last_of('.'));
		destPath = noSuffix + ".png";
		convert(srcpath, destPath);
	}

	FreeImage_DeInitialise();
	if (hasError)
	{
		system("pause");
	}
	
	return 1;
}