#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <string>
#include <vector>
#include <direct.h>
#include <io.h>
#include <cctype>
#include <algorithm>
#include <windows.h>

struct Vector2
{
	int x;
	int y;
	Vector2()
	{
		x = 0;
		y = 0;
	}
};
struct FloatVector2
{
	float x;
	float y;
	FloatVector2()
	{
		x = 0;
		y = 0;
	}
};

class Utility
{
public:
	static std::string getFileName(std::string str);
	static std::string getFileNameNoSuffix(std::string str);
	static std::string getParentDir(std::string dir);
	static void rightToLeft(std::string& str);
	static void split(std::string str, const char* deli, std::vector<std::string>* vec);
	static int stringToInt(std::string str)
	{
		return atoi(str.c_str());
	}
	static std::string floatToString(float f, int precision = 3);
	static float pow_f(float f, int p);
	static bool writeFile(std::string filePath, int length, const char* buffer);
	// 创建一个文件夹,path是一个不以/结尾的可直接访问的相对或者绝对的文件夹名
	static bool createFolder(std::string path);
	static bool isFileExist(std::string fullPath);
	static std::string intToString(int i, int limitLen = 0); // limitLen是字符串的最小长度,如果整数的位数不足最小长度,则会在前面加0
	static std::wstring ANSIToUnicode(const std::string& str);
	static std::string ANSIToUTF8(const std::string& str, const bool& addBOM);
	static std::string UnicodeToUTF8(const std::wstring& str);
	static void findFiles(const std::string& path, std::vector<std::string>& fileList, const std::string& pattern);
	static void findFiles(const std::string& path, std::vector<std::string>& fileList, const std::vector<std::string>& patterns);
	static Vector2 getImageSize(const std::string& fileName);
	static void convert(const std::string& src, const Vector2& targetSize, Vector2& imageNewSize, Vector2& imageTargetSize, FloatVector2& imageTargetPos);
};


#endif