#include <cstdio>
#include <cstdlib>
#include <direct.h>
#include <string>
#include <vector>
#include <windows.h>
#include <iostream>

#include "FreeImage.h"


#define MAX_PATH_LENGTH 1000


void findFiles(std::string path, std::vector<std::string>& fileList, std::vector<std::string> patterns)
{
	WIN32_FIND_DATA FindData;
	HANDLE hError;
	// 构造路径
	std::string pathName = path + "\\*.*";
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
		std::string fullname = path + "\\" + std::string(FindData.cFileName);
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

//return 1 if succ, -1 if failed
void convert(const char* src, const char* dst)
{
	FreeImage_Initialise(1);
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(src, 0);
	FIBITMAP* sourceBitmap = FreeImage_Load(fif, src, 0);
	if (sourceBitmap == NULL)
	{
		printf("freeImage Load file[%s] failed!\n", src);
		return;
	}

	bool hasAlpha = FreeImage_IsTransparent(sourceBitmap) == TRUE;
	if (hasAlpha)
	{
		return;
	}

	unsigned short width = FreeImage_GetWidth(sourceBitmap);
	unsigned short height = FreeImage_GetHeight(sourceBitmap);
	FIBITMAP* destBitmap = FreeImage_Allocate(width, height, 32);
	for (int y = 0; y < height; ++y)
	{
		BYTE* destBits = FreeImage_GetScanLine(destBitmap, y);
		BYTE* sourceBits = FreeImage_GetScanLine(sourceBitmap, y);
		for (int x = 0; x < width; ++x)
		{
			destBits[x * 4 + 0] = sourceBits[3 * x + 0];
			destBits[x * 4 + 1] = sourceBits[3 * x + 1];
			destBits[x * 4 + 2] = sourceBits[3 * x + 2];
			int alpha = 255;
			if (destBits[x * 4 + 0] == 0 && destBits[x * 4 + 1] == 0 && destBits[x * 4 + 2] == 0)
			{
				alpha = 0;
			}
			destBits[x * 4 + 3] = alpha;
		}
	}

	// 保存为png
	FreeImage_Save(FIF_PNG, destBitmap, dst);
	FreeImage_Unload(destBitmap);
}

int main(int argc, char **argv)
{
	std::vector<std::string> patterns;
	patterns.push_back(".png");
	std::vector<std::string> fileList;
	findFiles(".", fileList, patterns);

	int fileCount = fileList.size();
	for (int i = 0; i < fileCount; ++i)
	{
		convert(fileList[i].c_str(), (fileList[i] + ".png").c_str());
	}

	return 1;
}

