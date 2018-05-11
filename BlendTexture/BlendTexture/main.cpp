#include <iostream>
#include <vector>
#include <string>
#include <windows.h>

void FindFiles(const char * pathName, std::vector<std::string>& files, std::vector<std::string> patterns)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	CHAR PathBuffer[_MAX_PATH];

	strcpy_s(PathBuffer, _MAX_PATH, pathName);
	if (pathName[strlen(pathName) - 1] != '/')
	{
		strcat_s(PathBuffer, _MAX_PATH, "/*");
	}
	else
	{
		strcat_s(PathBuffer, _MAX_PATH, "*");
	}

	hFind = FindFirstFile(PathBuffer, &FindFileData);
	// 如果找不到文件夹就直接返回
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return;
	}
	while (::FindNextFile(hFind, &FindFileData))
	{
		// 过滤.和..
		if (strcmp(FindFileData.cFileName, ".") == 0
			|| strcmp(FindFileData.cFileName, "..") == 0)
		{
			continue;
		}

		// 构造完整路径
		std::string fullname = std::string(pathName) + "/" + std::string(FindFileData.cFileName);
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			FindFiles(fullname.c_str(), files, patterns);
		}
		else
		{
			int patternCount = patterns.size();
			for (int i = 0; i < patternCount; ++i)
			{
				if (fullname.length() >= patterns[i].length() && fullname.substr(fullname.length() - patterns[i].length(), patterns[i].length()) == patterns[i])
				{
					files.push_back(fullname);
				}
			}
		}
	}
	::FindClose(hFind);
}

const int PIXEL_SIZE = 4; // 一个像素占多少字节
const int HEADER_OFFSET = 8;

void main()
{
	std::vector<std::string> fileList;
	std::vector<std::string> patterns;
	patterns.push_back(".swi");
	FindFiles("./texture", fileList, patterns);
	int fileCount = fileList.size();
	unsigned char* blendBuffer = NULL;
	short blendWidth = 0;
	short blendHeight = 0;
	for (int i = 0; i < fileCount; ++i)
	{
		FILE* pFile = NULL;
		fopen_s(&pFile, fileList[i].c_str(), "rb");
		if (pFile == NULL)
		{
			// 无法打开文件
			return;
		}
		fseek(pFile, 0, SEEK_END);
		int fileSize = ftell(pFile);
		rewind(pFile);
		unsigned char* buffer = new unsigned char[fileSize];
		fread(buffer, sizeof(unsigned char), fileSize, pFile);
		fclose(pFile);
		short textureWidth = *(short*)(buffer + 4);
		short textureHeight = *(short*)(buffer + 6);
		if (blendBuffer == NULL)
		{
			blendWidth = textureWidth;
			blendHeight = textureHeight;
			blendBuffer = new unsigned char[blendWidth * blendHeight * PIXEL_SIZE];
			memcpy(blendBuffer, buffer + HEADER_OFFSET, blendWidth * blendHeight * PIXEL_SIZE);
		}
		else
		{
			if (textureWidth != blendWidth || blendHeight != blendHeight)
			{
				// 图片宽高不一致
				return;
			}
			int textureSize = blendWidth * blendHeight;
			for (int j = 0; j < textureSize; ++j)
			{
				int x = j % blendWidth;
				int y = j / blendWidth;
				for (int k = 0; k < PIXEL_SIZE; ++k)
				{
					int byteIndex = x * PIXEL_SIZE + k + y * blendWidth * PIXEL_SIZE;
					int pixel = blendBuffer[byteIndex] + buffer[HEADER_OFFSET + byteIndex];
					if (pixel > 255)
					{
						pixel = 255;
					}
					blendBuffer[byteIndex] = pixel;
				}
			}
		}
		delete[] buffer;
	}
	if (blendBuffer != NULL)
	{
		FILE* pFile = NULL;
		fopen_s(&pFile, "result.swi", "wb");
		if (pFile == NULL)
		{
			delete[] blendBuffer;
			return;
		}
		unsigned char* fileBuffer = new unsigned char[HEADER_OFFSET + blendWidth * blendHeight * 4];
		int writeIndex = 0;
		memcpy(fileBuffer + writeIndex, "swi ", 4);
		writeIndex += 4;
		memcpy(fileBuffer + writeIndex, &blendWidth, sizeof(blendWidth));
		writeIndex += sizeof(blendWidth);
		memcpy(fileBuffer + writeIndex, &blendHeight, sizeof(blendHeight));
		writeIndex += sizeof(blendHeight);
		memcpy(fileBuffer + writeIndex, blendBuffer, blendHeight * blendWidth * PIXEL_SIZE);
		writeIndex += blendHeight * blendWidth * PIXEL_SIZE;
		fwrite(fileBuffer, sizeof(unsigned char), writeIndex, pFile);
		fclose(pFile);
	}
}