#ifndef _FILE_UTILITY_H_
#define _FILE_UTILITY_H_

#include "ServerDefine.h"

class FileUtility
{
public:
	static string validPath(const string& path);
	static void findFiles(const string& path, txVector<string>& files, const string& patterns, bool recursive = true);
#if RUN_PLATFORM == PLATFORM_ANDROID
	static bool isDirectory(const string& path);
#endif
	static void findFiles(const string& path, txVector<string>& files, const txVector<string>& patterns = txVector<string>(), bool recursive = true);
	static void findFolders(const string& path, txVector<string>& folders, bool recursive = false);
	static void deleteFolder(const string& path);
	static bool deleteEmptyFolder(const string& path);
	static void deleteFile(const string& path);
	static bool isFileExist(const string& fullPath);
	static void renameFile(const string& curName, const string& newName);
	static void moveFile(const string& sourceFile, const string& destFile);
	// 将sourceFile拷贝到destFile,sourceFile和destFile都是带可直接访问的路径的文件名,overWrite指定当目标文件已经存在时是否要覆盖文件
	static bool copyFile(const string& sourceFile, const string& destFile, bool overWrite = true);
	// 创建一个文件夹,path是一个不以/结尾的可直接访问的相对或者绝对的文件夹名
	static bool createFolder(const string& path);
	static bool writeFile(string filePath, const char* buffer, int length, bool append = false);
	static bool writeFile(string filePath, const string& text, bool append = false);
	static bool writeFileSimple(const string& fileName, const char* buffer, int writeCount, bool append = false);
	static bool writeFileSimple(const string& fileName, const string& text, bool append = false);
	static char* openFile(const string& filePath, int* bufferSize, bool addZero);
	static string openTxtFile(const string& filePath);
	static char* openBinaryFile(const string& filePath, int* bufferSize);
};

#endif