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

//return 1 if succ, -1 if failed
int convert(std::string src, std::string dst)
{
	char filepath[128];
	_getcwd(filepath, sizeof(filepath));
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(src.c_str(), 0);
	FIBITMAP* bitmapSrc = FreeImage_Load(fif, src.c_str(), 0);
	if (bitmapSrc == 0)
	{
		printf("freeImage Load file[%s] failed!\n", src.c_str());
		return -1;
	}

	unsigned short width = FreeImage_GetWidth(bitmapSrc);
	unsigned short height = FreeImage_GetHeight(bitmapSrc);

	FIBITMAP* bitmapRet = FreeImage_Allocate(width / 2, height, 32);
	
	// 保存左半边图片的像素数据
	for (int y = 0; y < height; ++y)
	{
		BYTE* bits = FreeImage_GetScanLine(bitmapRet, y);
		for (int x = 0; x < width / 2; ++x)
		{
			RGBQUAD color;
			FreeImage_GetPixelColor(bitmapSrc, x, y, &color);
			bits[x * 4 + 0] = color.rgbBlue;
			bits[x * 4 + 1] = color.rgbGreen;
			bits[x * 4 + 2] = color.rgbRed;
			bits[x * 4 + 3] = color.rgbReserved;
		}
	}
	FreeImage_Unload(bitmapSrc);

	// 按原格式保存
	FreeImage_Save(fif, bitmapRet, dst.c_str());
	FreeImage_Unload(bitmapRet);
	return 1;
}

int main(int argc, char **argv)
{
	std::vector<std::string> patterns;
	patterns.push_back(".png");
	patterns.push_back(".jpg");
	patterns.push_back(".tga");
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
		convert(srcpath, destPath);
	}

	FreeImage_DeInitialise();
	if (hasError)
	{
		system("pause");
	}
	
	return 1;
}