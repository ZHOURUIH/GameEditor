#ifndef _FILE_UTILITY_H_
#define _FILE_UTILITY_H_

#include "CommonDefine.h"

class FileUtility
{
public:
	static void findFiles(const std::string& path, txVector<std::string>& files, const std::string& patterns, const bool& recursive = true);
	static void findFiles(std::string path, txVector<std::string>& files, txVector<std::string>& patterns = txVector<std::string>(), const bool& recursive = true);
	static void findFolders(std::string path, txVector<std::string>& folders, const bool& recursive = false);
	static void deleteFolder(const std::string& path);
	static bool isFileExist(const std::string& fullPath);
	// 将sourceFile拷贝到destFile,sourceFile和destFile都是带可直接访问的路径的文件名,overWrite指定当目标文件已经存在时是否要覆盖文件
	static bool copyFile(const std::string& sourceFile, const std::string& destFile, bool overWrite = true);
	// 创建一个文件夹,path是一个不以/结尾的可直接访问的相对或者绝对的文件夹名
	static bool createFolder(const std::string& path);
	static bool writeFile(std::string filePath, int length, const char* buffer, bool append = false);
	static char* openFile(const std::string& filePath, int* bufferSize, bool addZero);
	static std::string openTxtFile(const std::string& filePath);
	static char* openBinaryFile(const std::string& filePath, int* bufferSize);
};

#endif