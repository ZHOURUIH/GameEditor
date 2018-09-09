#include <iostream>
#include "Utility.h"
#include "FreeImage.h"

void deleteZeroPNG(const char* src)
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
	if (width * height == 1)
	{
		Utility::deleteFile(src);
	}
	FreeImage_Unload(sourceBitmap);
}

void main()
{
	std::vector<std::string> fileList;
	Utility::findFiles("./", fileList, ".png", false);
	int fileCount = fileList.size();
	for (int i = 0; i < fileCount; ++i)
	{
		deleteZeroPNG(fileList[i].c_str());
	}
}