#ifndef _FILE_UTILITY_H_
#define _FILE_UTILITY_H_

#include "CommonDefine.h"

class FileUtility
{
public:
	static void findFiles(const char * pathName, txVector<std::string>& files, const txVector<std::string>& patterns = txVector<std::string>(), bool recursive = true);
	static void findFolers(std::string path, txVector<std::string>& folders, const bool& recursive = false);
	static void deleteFolder(const std::string& path);
	static void deleteFile(const std::string& fileName);
	static bool isFileExist(std::string fullPath);
	// 将sourceFile拷贝到destFile,sourceFile和destFile都是带可直接访问的路径的文件名,overWrite指定当目标文件已经存在时是否要覆盖文件
	static bool copyFile(std::string sourceFile, std::string destFile, bool overWrite = true);
	// 创建一个文件夹,path是一个不以/结尾的可直接访问的相对或者绝对的文件夹名
	static bool createFolder(std::string path);
	static bool openFile(std::string filePath, int& length, char*& buffer, const bool& addZero);
	static bool writeFile(std::string filePath, int length, char* buffer);
	static int getFileSize(std::string filePath);
};

#endif