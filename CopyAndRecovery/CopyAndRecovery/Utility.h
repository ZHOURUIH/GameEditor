#pragma once

#include <string>
#include <vector>

#include "txConfig.h"

#define TX_MAX(x, y) ((x) > (y) ? (x) : (y))
#define TX_MIN(x, y) ((x) < (y) ? (x) : (y))

class Utility
{
public:
#if RUN_PLATFORM == PLATFORM_ANDROID
	static bool IsDirectory(char *pszName);
	static void findFiles(const char * pathName, std::vector<std::string>& files, std::vector<std::string> patterns = std::vector<std::string>());
#elif RUN_PLATFORM == PLATFORM_WINDOWS
	static void findFiles(const char * pathName, std::vector<std::string>& files, std::vector<std::string> patterns = std::vector<std::string>());
	static void deleteFolder(const std::string& path, bool deleteFolderSelf);
	static bool deleteFile(const std::string& fileName);
	static bool deleteEmptyFolder(const std::string& path, std::vector<std::string>& emptyFolder);
#endif
	// 将sourceFile拷贝到destFile,sourceFile和destFile都是带可直接访问的路径的文件名,overWrite指定当目标文件已经存在时是否要覆盖文件
	static bool copyFile(const std::string& sourceFile, const std::string& destFile, bool overWrite = true);
	static bool moveFile(const std::string& sourceFile, const std::string& destFile);
	// 创建一个文件夹,path是一个不以/结尾的可直接访问的相对或者绝对的文件夹名
	static bool createFolder(const std::string& path);
	static bool writeFile(std::string filePath, int length, const char* buffer);

	static void rightToLeft(std::string& str)
	{
		int pathLength = str.length();
		for (int i = 0; i < pathLength; ++i)
		{
			if ('\\' == str[i])
			{
				str[i] = '/';
			}
		}
	}

	static bool Utility::isFileExist(const std::string& fullPath);

	static std::string intToString(int i, int limitLen = 0); // limitLen是字符串的最小长度,如果整数的位数不足最小长度,则会在前面加0
	static int stringToInt(std::string str)
	{
		return atoi(str.c_str());
	}
	static float pow_f(float f, int p);
	static std::string floatToString(float f, int precision);	//precision -> 精度,保留的小数的位数
	static float stringToFloat(const std::string& str)
	{
		return (float)atof(str.c_str());
	}

	static void split(std::string str, const char * deli, std::vector<std::string>& vec);
	// 判断oriString是否以pattern结尾
	static bool endWith(const std::string& oriString, const std::string& pattern)
	{
		if (oriString.length() < pattern.length())
		{
			return false;
		}
		std::string endString = oriString.substr(oriString.length() - pattern.length(), pattern.length());
		return endString == pattern;
	}
	// 判断oriString是否以pattern开头
	static bool startWith(const std::string& oriString, const std::string& pattern)
	{
		if (oriString.length() < pattern.length())
		{
			return false;
		}
		std::string startString = oriString.substr(0, pattern.length());
		return startString == pattern;
	}

	static std::string getParentDir(std::string dir)
	{
		rightToLeft(dir);
		int pos = dir.find_last_of('/');
		std::string tempDir = dir;
		if (-1 != pos)
		{
			tempDir = dir.substr(0, pos);
		}

		return tempDir;
	}

	static std::string getFileName(std::string str)
	{
		rightToLeft(str);
		int dotPos = str.find_last_of('/');
		if (dotPos != -1)
		{
			return str.substr(dotPos + 1, str.length() - 1);
		}
		return str;
	}

	static std::string getFileNameNoSuffix(std::string str)
	{
		rightToLeft(str);
		int namePos = str.find_last_of('/');
		int dotPos = str.find_last_of('.');
		if (dotPos != -1)
		{
			return str.substr(namePos + 1, dotPos - namePos - 1);
		}

		return str;
	}
	static char* openFile(const std::string& filePath, int* bufferSize, const bool& addZero);
	static std::string openTxtFile(const std::string& filePath);
	static char* openBinaryFile(const std::string& filePath, int* bufferSize);
};

