#include "Utility.h"
#include "txMemoryTrace.h"
#include "ToolCoreLog.h"
#include "ToolCoreUtility.h"

void FileUtility::findFiles(const std::string& pathName, txVector<std::string>& files, const std::string& patterns, const bool& recursive)
{
	txVector<std::string> patternList;
	patternList.push_back(patterns);
	findFiles(pathName, files, patternList, recursive);
}

void FileUtility::findFiles(std::string path, txVector<std::string>& files, txVector<std::string>& patterns, const bool& recursive)
{
	if (path.length() == 0)
	{
		return;
	}
	// 不以/结尾,则加上/
	if (path[path.length() - 1] != '/')
	{
		path += "/";
	}
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = FindFirstFileA((path + "*").c_str(), &FindFileData);
	// 如果找不到文件夹就直接返回
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return;
	}
	do
	{
		// 过滤.和..
		if (strcmp(FindFileData.cFileName, ".") == 0
			|| strcmp(FindFileData.cFileName, "..") == 0)
		{
			continue;
		}

		// 构造完整路径
		std::string fullname = path + std::string(FindFileData.cFileName);
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (recursive)
			{
				findFiles(fullname.c_str(), files, patterns, recursive);
			}
		}
		else
		{
			int patternCount = patterns.size();
			if (patternCount > 0)
			{
				for (int i = 0; i < patternCount; ++i)
				{
					if (StringUtility::endWith(fullname, patterns[i]))
					{
						files.push_back(fullname);
					}
				}
			}
			else
			{
				files.push_back(fullname);
			}
		}
	} while (::FindNextFileA(hFind, &FindFileData));
	::FindClose(hFind);
}

void FileUtility::findFolders(std::string path, txVector<std::string>& folders, const bool& recursive)
{
	if (path.length() == 0)
	{
		return;
	}
	// 不以/结尾,则加上/
	if (path[path.length() - 1] != '/')
	{
		path += "/";
	}
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = FindFirstFileA((path + "*").c_str(), &FindFileData);
	// 如果找不到文件夹就直接返回
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return;
	}
	do
	{
		// 过滤.和..
		if (strcmp(FindFileData.cFileName, ".") == 0
			|| strcmp(FindFileData.cFileName, "..") == 0)
		{
			continue;
		}

		// 构造完整路径
		std::string fullname = path + std::string(FindFileData.cFileName);
		// 是文件夹则先放入列表,然后判断是否递归查找
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			folders.push_back(fullname);
			if (recursive)
			{
				findFolders(fullname.c_str(), folders, recursive);
			}
		}
	} while (::FindNextFileA(hFind, &FindFileData));
	::FindClose(hFind);
}

void FileUtility::deleteFolder(const std::string& path)
{
	WIN32_FIND_DATAA FindData;
	// 构造路径
	std::string pathName = path + "/*.*";
	HANDLE hFind = FindFirstFileA(pathName.c_str(), &FindData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return;
	}
	while (::FindNextFileA(hFind, &FindData))
	{
		// 过虑.和..
		if (strcmp(FindData.cFileName, ".") == 0
			|| strcmp(FindData.cFileName, "..") == 0)
		{
			continue;
		}

		// 构造完整路径
		std::string fullname = path + "/" + std::string(FindData.cFileName);
		// 如果是文件夹,则递归删除文件夹
		if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			deleteFolder(fullname);
		}
		// 如果是文件,则直接删除文件
		else
		{
			DeleteFileA(fullname.c_str());
		}
	}
	::FindClose(hFind);
	// 删除文件夹自身
	RemoveDirectoryA(path.c_str());
}

bool FileUtility::isFileExist(const std::string& fullPath)
{
	if (fullPath == "." || fullPath == "..")
	{
		return true;
	}
	int ret = _access(fullPath.c_str(), 0);
	return ret == 0;
}

bool FileUtility::copyFile(const std::string& sourceFile, const std::string& destFile, bool overWrite)
{
	// 如果目标文件所在的目录不存在,则先创建目录
	std::string parentDir = StringUtility::getParentDir(destFile);
	createFolder(parentDir);
	return CopyFileA(sourceFile.c_str(), destFile.c_str(), !overWrite) == TRUE;
}

bool FileUtility::createFolder(const std::string& path)
{
	// 如果有上一级目录,并且上一级目录不存在,则先创建上一级目录
	std::string parentDir = StringUtility::getParentDir(path);
	if (parentDir != path)
	{
		createFolder(parentDir);
	}
	// 如果文件不存在则创建文件
	if (!isFileExist(path))
	{
		if (0 != _mkdir(path.c_str()))
		{
			return false;
		}
	}
	return true;
}

bool FileUtility::writeFile(std::string filePath, int length, const char* buffer, bool append)
{
	StringUtility::rightToLeft(filePath);
	std::string tempPath = filePath;
	int pos = filePath.find_last_of('/');
	if (pos != -1)
	{
		tempPath = filePath.substr(0, pos);
	}
	if (!createFolder(tempPath))
	{
		TOOL_CORE_ERROR("error : can not create folder, name : %s", filePath.c_str());
		return false;
	}
	char* openMode = "";
	if (append)
	{
		// 不清空文件,并将文件指示器移到文件末尾
		openMode = "ab+";
	}
	else
	{
		// 清空文件,并将文件指示器移到文件开头
		openMode = "wb";
	}
	FILE* pFile = NULL;
	fopen_s(&pFile, filePath.c_str(), openMode);
	if (pFile == NULL)
	{
		TOOL_CORE_ERROR("error : can not write file, name : %s", filePath.c_str());
		return false;
	}
	if (buffer != NULL && length > 0)
	{
		fwrite(buffer, sizeof(char), length, pFile);
	}
	fclose(pFile);
	return true;
}

char* FileUtility::openFile(const std::string& filePath, int* bufferSize, bool addZero)
{
	FILE* pFile = NULL;
	fopen_s(&pFile, filePath.c_str(), "rb");
	if (pFile == NULL)
	{
		return NULL;
	}
	fseek(pFile, 0, SEEK_END);
	int fileSize = ftell(pFile);
	rewind(pFile);
	int bufferLen = addZero ? fileSize + 1 : fileSize;
	if (bufferSize != NULL)
	{
		*bufferSize = bufferLen;
	}
	char* buffer = TRACE_NEW_ARRAY(char, bufferLen, buffer);
	if (buffer != NULL)
	{
		fread(buffer, sizeof(char), fileSize, pFile);
	}
	fclose(pFile);
	return buffer;
}

std::string FileUtility::openTxtFile(const std::string& filePath)
{
	char* buffer = openFile(filePath, NULL, true);
	if (buffer == NULL)
	{
		return EMPTY_STRING;
	}
	std::string str(buffer);
	TRACE_DELETE_ARRAY(buffer);
	return str;
}

char* FileUtility::openBinaryFile(const std::string& filePath, int* bufferSize)
{
	return openFile(filePath, bufferSize, false);
}