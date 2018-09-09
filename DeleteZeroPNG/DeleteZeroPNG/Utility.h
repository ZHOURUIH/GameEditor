#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <string>
#include <vector>
#include <map>
#include <windows.h>
#include <io.h>
#include <direct.h>

class Utility
{
public:
	static std::string validPath(const std::string& path);
	static void findFiles(const std::string& path, std::vector<std::string>& files, const std::string& patterns, bool recursive = true);
	static void findFiles(const std::string& path, std::vector<std::string>& files, const std::vector<std::string>& patterns = std::vector<std::string>(), bool recursive = true);
	static bool endWith(const std::string& oriString, const std::string& pattern, bool sensitive = true);
	static void strToLower(std::string& str);
	static void moveFile(const std::string& source, const std::string& dest);
	static std::string intToString(int i, int limitLen = 0);
	static int stringToInt(const std::string& str) { return atoi(str.c_str()); }
	static bool createFolder(const std::string& path);
	static bool isFileExist(const std::string& fullPath);
	static std::string getFilePath(std::string dir);
	static void rightToLeft(std::string& str);
	static bool copyFile(const std::string& sourceFile, const std::string& destFile, bool overWrite = true);
	static std::string getFileName(std::string str);
	static void deleteFile(const std::string& fileName);
};

#endif
