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
int convert(char* src, char* dst)
{
	//open src picture
	FILE *fp;
	FREE_IMAGE_FORMAT fif;
	FreeImage_Initialise(1);
	fopen_s(&fp, dst, "wb");
	if (NULL == fp)
	{
		printf("the des File[%s] open error!\n", dst);
		return -1;
	}
	char filepath[128];
	_getcwd(filepath, sizeof(filepath));
	fif = FreeImage_GetFileType(src, 0);
	FIBITMAP *bitmap = FreeImage_Load(fif, src, 0);
	if (bitmap == 0)
	{
		printf("freeImage Load file[%s] failed!\n", src);
		return -1;
	}

	char head[5] = "SWI ";
	fwrite(head, 4, 1, fp);

	unsigned short width = FreeImage_GetWidth(bitmap);
	unsigned short height = FreeImage_GetHeight(bitmap);
	fwrite((void *) &width, 2, 1, fp);
	fwrite((void *) &height, 2, 1, fp);

	bool hasAlpha = FreeImage_IsTransparent(bitmap) == TRUE;

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			RGBQUAD color;
			FreeImage_GetPixelColor(bitmap, x, y, &color);
			fputc(color.rgbRed, fp);
			fputc(color.rgbGreen, fp);
			fputc(color.rgbBlue, fp);
			if (!hasAlpha)
			{
				color.rgbReserved = 255;
			}
			fputc(color.rgbReserved, fp);
		}
	}
	FreeImage_Unload(bitmap);
	fclose(fp);
	FreeImage_DeInitialise();
	return 1;
}

int main(int argc, char **argv)
{
	char srcpath[MAX_PATH_LENGTH];
	char dstpath[MAX_PATH_LENGTH];

	std::vector<std::string> patterns;
	patterns.push_back(".png");
	patterns.push_back(".tga");
	patterns.push_back(".jpg");
	patterns.push_back(".jpeg");
	patterns.push_back(".bmp");
	patterns.push_back(".psd");
	patterns.push_back(".tiff");
	patterns.push_back(".dds");
	std::vector<std::string> fileList;
	findFiles(".", fileList, patterns);

	int fileCount = fileList.size();
	for (int i = 0; i < fileCount; ++i)
	{
		std::string temp = fileList[i];
		sprintf_s(srcpath, MAX_PATH_LENGTH, "%s", temp.c_str());
		int pointPosition = temp.find_last_of('.');
		std::string temp2;
		for (int i = 0; i < pointPosition; ++i)
		{
			char ch[2] = "";
			sprintf_s(ch, 2, "%c", temp[i]);
			temp2.append(ch);
		}
		temp2.append(".swi");

		sprintf_s(dstpath, MAX_PATH_LENGTH, "%s", temp2.c_str());
		convert(srcpath, dstpath);
	}

	return 1;
}

