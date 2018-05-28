#include "txFileUtility.h"
#include "Utility.h"

void txFileUtility::findFiles(const std::string& pathName, txVector<std::string>& files, const std::string& patterns, const bool& recursive)
{
	txVector<std::string> patternList;
	patternList.push_back(patterns);
	findFiles(pathName, files, patternList, recursive);
}
#if RUN_PLATFORM == PLATFORM_ANDROID
// 判断是否为目录
bool txFileUtility::isDirectory(const std::string& pszName)
{
	struct stat S_stat;
	// 取得文件状态
	if (lstat(pszName, &S_stat) < 0)
	{
		return false;
	}
	// 判断文件是否为文件夹
	return S_ISDIR(S_stat.st_mode);
}

void txFileUtility::findFiles(const std::string& path, txVector<std::string>& files, txVector<std::string>& patterns, const bool& recursive)
{
#ifdef LOAD_FROM_ASSETMANAGER
	txVector<std::string> fileName = ASS_getFileList((char*)path.c_str());
	int fileCount = fileName.size();
	FOR_STL(fileName, int i = 0; i < fileCount; ++i)
	{
		const std::string& fullName = fileName[i];
		// 判断是否符合后缀过滤
		int patternCount = patterns.size();
		if(patternCount > 0)
		{
			FOR_STL(patterns, int j = 0; j < patternCount; ++j)
			{
				if (txStringUtility::endWith(fullName, patterns[j], false))
				{
					files.push_back(fullName);
					break;
				}
			}
			END_FOR_STL(patterns);
		}
		else
		{
			files.push_back(fullName);
		}
	}
	END_FOR_STL(fileName);

#else
	DIR* pDir = opendir(path.c_str());
	if (pDir == NULL)
	{
		return;
	}
	char szTmpPath[1024];
	dirent* pDirent = NULL;
	while ((pDirent = readdir(pDir)) != NULL)
	{
		//如果是.或者..跳过
		if (std::string(pDirent->d_name) == "." || std::string(pDirent->d_name) == "..")
		{
			continue;
		}
		//判断是否为文件夹
		memset(szTmpPath, 0, 1024);
		SPRINTF(szTmpPath, 1024, "%s/%s", path.c_str(), pDirent->d_name);
		if (isDirectory(szTmpPath))
		{
			if (recursive)
			{
				//如果是文件夹则进行递归
				findFiles(szTmpPath, files, patterns, recursive);
			}
		}
		else
		{
			int patternCount = patterns.size();
			if(patternCount > 0)
			{
				FOR_STL(patterns, int i = 0; i < patternCount; ++i)
				{
					if (txStringUtility::endWith(szTmpPath, patterns[i], false))
					{
						files.push_back(szTmpPath);
						break;
					}
				}
				END_FOR_STL(patterns);
			}
			else
			{
				files.push_back(szTmpPath);
			}
		}
	}
	closedir(pDir);
#endif
}

void txFileUtility::findFolders(const std::string& path, txVector<std::string>& folders, const bool& recursive = false)
{
	struct dirent* pDirent;
	DIR* pDir = opendir(path.c_str());
	if (pDir == NULL)
	{
		return;
	}
	char szTmpPath[1024];
	while ((pDirent = readdir(pDir)) != NULL)
	{
		//如果是.或者..跳过
		if (std::string(pDirent->d_name) == "." || std::string(pDirent->d_name) == "..")
		{
			continue;
		}
		//判断是否为文件夹
		memset(szTmpPath, 0, 1024);
		SPRINTF(szTmpPath, 1024, "%s/%s", path.c_str(), pDirent->d_name);
		// 如果是文件夹,则先将文件夹放入列表,然后判断是否递归查找
		if (isDirectory(szTmpPath))
		{
			folders.push_back(szTmpPath);
			if (recursive)
			{
				findFolders(szTmpPath, folders, recursive);
			}
		}
	}
	closedir(pDir);
}

#elif RUN_PLATFORM == PLATFORM_WINDOWS
void txFileUtility::findFiles(std::string path, txVector<std::string>& files, txVector<std::string>& patterns, const bool& recursive)
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
				FOR_STL(patterns, int i = 0; i < patternCount; ++i)
				{
					if (txStringUtility::endWith(fullname, patterns[i], false))
					{
						files.push_back(fullname);
					}
				}
				END_FOR_STL(patterns);
			}
			else
			{
				files.push_back(fullname);
			}
		}
	}while (::FindNextFileA(hFind, &FindFileData));
	::FindClose(hFind);
}

void txFileUtility::findFolders(std::string path, txVector<std::string>& folders, const bool& recursive)
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

void txFileUtility::deleteFolder(const std::string& path)
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
#endif

bool txFileUtility::isFileExist(const std::string& fullPath)
{
#if RUN_PLATFORM == PLATFORM_WINDOWS
	int ret = _access(fullPath.c_str(), 0);
#elif RUN_PLATFORM == PLATFORM_ANDROID
#ifdef LOAD_FROM_ASSETMANAGER
	int ret = ASS_isFileExist((char*)fullPath.c_str()) ? 0 : -1;
#else
	int ret = access(fullPath.c_str(), 0);
#endif
#endif
	return ret == 0;
}

bool txFileUtility::copyFile(const std::string& sourceFile, const std::string& destFile, bool overWrite)
{
	// 如果目标文件所在的目录不存在,则先创建目录
	std::string parentDir = txStringUtility::getFilePath(destFile);
	createFolder(parentDir);
#if RUN_PLATFORM == PLATFORM_WINDOWS
	return CopyFileA(sourceFile.c_str(), destFile.c_str(), !overWrite) == TRUE;
#elif RUN_PLATFORM == PLATFORM_ANDROID
	return false;
#endif
}

bool txFileUtility::createFolder(const std::string& path)
{
	// 如果有上一级目录,并且上一级目录不存在,则先创建上一级目录
	std::string parentDir = txStringUtility::getFilePath(path);
	if (parentDir != path)
	{
		createFolder(parentDir);
	}
	// 如果文件不存在则创建文件
	if (!isFileExist(path))
	{
#ifdef _USE_SAFE_API
		if (0 != _mkdir(path.c_str()))
#else
		if (0 != mkdir(path.c_str(), S_IRUSR | S_IWUSR | S_IXUSR))
#endif
		{
			return false;
		}
	}
	return true;
}

bool txFileUtility::writeFile(std::string filePath, int length, const char* buffer)
{
#ifdef LOAD_FROM_ASSETMANAGER
	return false;
#else
	txStringUtility::rightToLeft(filePath);
	int pos = filePath.find_last_of('/');
	if (-1 != pos)
	{
		std::string dirPath = filePath.substr(0, pos);
		if (!createFolder(dirPath))
		{
			ENGINE_ERROR("error : can not create folder, name : %s", dirPath.c_str());
			return false;
		}
	}
	else
	{
		if (!createFolder(filePath))
		{
			ENGINE_ERROR("error : can not create folder, name : %s", filePath.c_str());
			return false;
		}
	}

#ifdef _USE_SAFE_API
	FILE* pFile = NULL;
	fopen_s(&pFile, filePath.c_str(), "wb");
#else
	FILE* pFile = fopen(filePath.c_str(), "wb");
#endif
	if (pFile == NULL)
	{
		ENGINE_ERROR("error : can not write file, name : %s", filePath.c_str());
		return false;
	}
	if (buffer != NULL && length > 0)
	{
		fwrite(buffer, sizeof(char), length, pFile);
	}
	fclose(pFile);
#endif
	return true;
}

char* txFileUtility::openFile(std::string filePath, int* bufferSize, const bool& addZero)
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

std::string txFileUtility::openTxtFile(const std::string& filePath)
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

char* txFileUtility::openBinaryFile(const std::string& filePath, int* bufferSize)
{
	return openFile(filePath, bufferSize, false);
}