#ifndef _UTILITY_H
#define _UTILITY_H

#include <vector>
#include <string>
#include <map>
#include <stdlib.h>

#include "CommonDefine.h"

class Utility
{
public:
	Utility(){}
	static void findFiles(const char * pathName, std::vector<std::string>& files, std::vector<std::string> patterns, bool recursive = true);
	static CODE_TYPE getFileCodeType(const char* fileBuffer, int length);
	
	static bool createFolder(std::string path);
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
	static bool isFileExist(std::string fullPath);

	static void readFile(std::string filePath, char* &bufferOut, int& fileLengthOut);
	static bool writeFile(std::string filePath, int length, const char* buffer);

	static char* addUTF8BOM(char* buffer, int strLength, int& byteLengthOut, bool needEndNull = false);
	static wchar_t* addUNICODEBOM(wchar_t* buffer, int strLength, int& byteLengthOut, bool needEndNull = false);
	static char* deleteUTF8BOM(char* buffer, int strLength, int& byteLengthOut, bool needEndNull = false);
	static wchar_t* deleteUNICODEBOM(char* buffer, int byteLength, int& byteLengthOut, bool needEndNull = false);

	static wchar_t* ANSIToUnicode(const char* buffer, int& stringLengthOut);
	static char* UnicodeToANSI(const wchar_t* buffer, int& stringLengthOut);
	static char* UnicodeToUTF8(const wchar_t* buffer, int& stringLengthOut);
	static wchar_t* UTF8ToUnicode(const char* buffer, int& stringLengthOut);
	static char* ANSIToUTF8(const char* buffer, int& stringLengthOut);
	static char* UTF8ToANSI(const char* buffer, int& stringLengthOut);
	static void split(std::string str, const char* deli, std::vector<std::string>* vec);
};
#endif