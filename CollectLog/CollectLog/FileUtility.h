#ifndef _FILE_UTILITY_H_
#define _FILE_UTILITY_H_

#include "Header.h"

class FileUtility
{
public:
	static std::string validPath(const std::string& path);
	static void findFiles(std::string pathName, std::vector<std::string>& files, const std::vector<std::string>& patterns = std::vector<std::string>(), const bool& recursive = true);
	static void findFolers(std::string path, std::vector<std::string>& folders, const bool& recursive = false);
	static void deleteFolder(std::string path);
	static bool deleteFile(const std::string& fileName);
	static bool deleteEmptyFolder(std::string path);	// 返回值为是否删除成功
	static bool renameFile(const std::string& fileName, const std::string& newName);
	static bool isFileExist(std::string fullPath);
	// 将sourceFile拷贝到destFile,sourceFile和destFile都是带可直接访问的路径的文件名,overWrite指定当目标文件已经存在时是否要覆盖文件
	static bool copyFile(std::string sourceFile, std::string destFile, bool overWrite = true);
	// 创建一个文件夹,path是一个不以/结尾的可直接访问的相对或者绝对的文件夹名
	static bool createFolder(std::string path);
	static bool openFile(std::string filePath, int& length, char*& buffer, const bool& addZero);
	static bool writeFile(std::string filePath, int length, char* buffer, const bool& appendData = false);
	static int getFileSize(const std::string& filePath);
};

#endif