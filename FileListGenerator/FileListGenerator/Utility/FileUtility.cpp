#include "FileUtility.h"
#include "StringUtility.h"

void FileUtility::findFiles(const char * pathName, txVector<std::string>& files, const txVector<std::string>& patterns, bool recursive)
{
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	std::string path(pathName);
	if (path.length() == 0)
	{
		return;
	}
	// 不以/结尾,则加上/
	if (path[path.length() - 1] != '/')
	{
		path += "/";
	}

	hFind = FindFirstFileA((path + "*").c_str(), &FindFileData);
	// 如果找不到文件夹就直接返回
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return;
	}
	while (::FindNextFileA(hFind, &FindFileData))
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
					if (fullname.length() >= patterns[i].length() && fullname.substr(fullname.length() - patterns[i].length(), patterns[i].length()) == patterns[i])
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
	}
	::FindClose(hFind);
}

void FileUtility::findFolers(std::string path, txVector<std::string>& folders, const bool& recursive)
{
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	if (path.length() == 0)
	{
		return;
	}
	// 不以/结尾,则加上/
	if (path[path.length() - 1] != '/')
	{
		path += "/";
	}

	hFind = FindFirstFileA((path + "*").c_str(), &FindFileData);
	// 如果找不到文件夹就直接返回
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return;
	}
	while (::FindNextFileA(hFind, &FindFileData))
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
				findFolers(fullname.c_str(), folders, recursive);
			}
		}
	}
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
		// 过滤.和..
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
			deleteFile(fullname);
		}
	}
	::FindClose(hFind);
	// 删除文件夹自身
	BOOL ret = RemoveDirectoryA(path.c_str());
}

void FileUtility::deleteFile(const std::string& fileName)
{
	::DeleteFileA(fileName.c_str());
}

bool FileUtility::isFileExist(std::string fullPath)
{
	int ret = _access(fullPath.c_str(), 0);
	return ret == 0;
}

bool FileUtility::copyFile(std::string sourceFile, std::string destFile, bool overWrite)
{
	// 如果目标文件所在的目录不存在,则先创建目录
	std::string parentDir = StringUtility::getParentDir(destFile);
	createFolder(parentDir);
#if RUN_PLATFORM == PLATFORM_WINDOWS
	return CopyFileA(sourceFile.c_str(), destFile.c_str(), !overWrite) == TRUE;
#elif RUN_PLATFORM == PLATFORM_ANDROID
	return false;
#endif
}

bool FileUtility::createFolder(std::string path)
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

bool FileUtility::openFile(std::string filePath, int& length, char*& buffer, const bool& addZero)
{
	FILE* pFile = NULL;
	fopen_s(&pFile, filePath.c_str(), "rb");
	if (pFile == NULL)
	{
		return false;
	}
	fseek(pFile, 0, SEEK_END);
	length = ftell(pFile);
	rewind(pFile);
	int bufferSize = length;
	if (addZero)
	{
		bufferSize += 1;
	}
	buffer = TRACE_NEW_ARRAY(char, bufferSize, buffer);
	memset(buffer, 0, bufferSize);
	fread(buffer, sizeof(char), length, pFile);
	fclose(pFile);
	return true;
}

bool FileUtility::writeFile(std::string filePath, int length, char* buffer)
{
	if (length <= 0 || NULL == buffer)
	{
		return false;
	}

	StringUtility::rightToLeft(filePath);

	int pos = filePath.find_last_of('/');
	if (-1 != pos)
	{
		std::string dirPath = filePath.substr(0, pos);
		if (!createFolder(dirPath))
		{
			return false;
		}
	}

	FILE* pFile = NULL;
	fopen_s(&pFile, filePath.c_str(), "wb");
	if (pFile == NULL)
	{
		return false;
	}
	fwrite(buffer, sizeof(char), length, pFile);
	fclose(pFile);
	return true;
}

int FileUtility::getFileSize(std::string filePath)
{
	struct _stat info;
	_stat(filePath.c_str(), &info);
	return info.st_size;
}