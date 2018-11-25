#include "Utility.h"

string FileUtility::validPath(const string& path)
{
	string temp = path;
	if (temp.length() > 0)
	{
		// 不以/结尾,则加上/
		if (temp[temp.length() - 1] != '/')
		{
			temp += "/";
		}
	}
	return temp;
}

void FileUtility::findFiles(const string& pathName, txVector<string>& files, const string& patterns, bool recursive)
{
	txVector<string> patternList;
	patternList.push_back(patterns);
	findFiles(pathName, files, patternList, recursive);
}
#if RUN_PLATFORM == PLATFORM_ANDROID
// 判断是否为目录
bool FileUtility::isDirectory(const string& pszName)
{
	struct stat S_stat;
	// 取得文件状态
	if (lstat(pszName.c_str(), &S_stat) < 0)
	{
		return false;
	}
	// 判断文件是否为文件夹
	return S_ISDIR(S_stat.st_mode);
}

void FileUtility::findFiles(const string& path, txVector<string>& files, const txVector<string>& patterns, bool recursive)
{
#ifdef LOAD_FROM_ASSETMANAGER
	txVector<string> fileName = ASS_getFileList((char*)path.c_str());
	int fileCount = fileName.size();
	FOR(fileName, int i = 0; i < fileCount; ++i)
	{
		const string& fullName = fileName[i];
		// 判断是否符合后缀过滤
		int patternCount = patterns.size();
		if(patternCount > 0)
		{
			for(int i = 0; i < patternCount; ++i)
			{
				if (txStringUtility::endWith(fullName, patterns[j], false))
				{
					files.push_back(fullName);
					break;
				}
			}
		}
		else
		{
			files.push_back(fullName);
		}
	}
	END(fileName);

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
		if (string(pDirent->d_name) == "." || string(pDirent->d_name) == "..")
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
				for(int i = 0; i < patternCount; ++i)
				{
					if (txStringUtility::endWith(szTmpPath, patterns[i], false))
					{
						files.push_back(szTmpPath);
						break;
					}
				}
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

void FileUtility::findFolders(const string& path, txVector<string>& folders, bool recursive)
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
		if (string(pDirent->d_name) == "." || string(pDirent->d_name) == "..")
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
void FileUtility::deleteFile(const string& path)
{
	remove(path.c_str());
}

#elif RUN_PLATFORM == PLATFORM_WINDOWS
void FileUtility::findFiles(const string& path, txVector<string>& files, const txVector<string>& patterns, bool recursive)
{
	string tempPath = validPath(path);
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = FindFirstFileA((tempPath + "*").c_str(), &FindFileData);
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
		string fullname = tempPath + string(FindFileData.cFileName);
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
				for(int i = 0; i < patternCount; ++i)
				{
					if (StringUtility::endWith(fullname, patterns[i], false))
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
	}while (::FindNextFileA(hFind, &FindFileData));
	::FindClose(hFind);
}

void FileUtility::findFolders(const string& path, txVector<string>& folders, bool recursive)
{
	string tempPath = validPath(path);
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = FindFirstFileA((tempPath + "*").c_str(), &FindFileData);
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
		string fullname = tempPath + string(FindFileData.cFileName);
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

void FileUtility::deleteFolder(const string& path)
{
	WIN32_FIND_DATAA FindData;
	// 构造路径
	string pathName = path + "/*.*";
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
		string fullname = path + "/" + string(FindData.cFileName);
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

bool FileUtility::deleteEmptyFolder(const string& path)
{
	WIN32_FIND_DATAA FindData;
	// 构造路径
	string pathName = path + "/*.*";
	HANDLE hFind = FindFirstFileA(pathName.c_str(), &FindData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return true;
	}
	bool isEmpty = true;
	while (::FindNextFileA(hFind, &FindData))
	{
		// 过虑.和..
		if (strcmp(FindData.cFileName, ".") == 0
			|| strcmp(FindData.cFileName, "..") == 0)
		{
			continue;
		}

		// 构造完整路径
		string fullname = path + "/" + string(FindData.cFileName);
		// 如果是文件夹,则递归删除空文件夹
		if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			// 如果删除该空文件夹时,返回该文件夹不为空,则当前文件夹也不为空
			isEmpty = deleteEmptyFolder(fullname) && isEmpty;
		}
		// 如果是文件,则该文件夹不为空
		else
		{
			isEmpty = false;
		}
	}
	::FindClose(hFind);
	// 删除文件夹自身
	if (isEmpty)
	{
		RemoveDirectoryA(path.c_str());
	}
	return isEmpty;
}

void FileUtility::deleteFile(const string& path)
{
	DeleteFileA(path.c_str());
}
#endif

bool FileUtility::isFileExist(const string& fullPath)
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

void FileUtility::renameFile(const string& curName, const string& newName)
{
	rename(curName.c_str(), newName.c_str());
}

void FileUtility::moveFile(const string& sourceFile, const string& destFile)
{
	// 如果目标文件所在的目录不存在,则先创建目录
	createFolder(StringUtility::getFilePath(destFile));
#if RUN_PLATFORM == PLATFORM_WINDOWS
	MoveFileA(sourceFile.c_str(), destFile.c_str());
#endif
}

bool FileUtility::copyFile(const string& sourceFile, const string& destFile, bool overWrite)
{
	// 如果目标文件所在的目录不存在,则先创建目录
	createFolder(StringUtility::getFilePath(destFile));
#if RUN_PLATFORM == PLATFORM_WINDOWS
	return CopyFileA(sourceFile.c_str(), destFile.c_str(), !overWrite) == TRUE;
#elif RUN_PLATFORM == PLATFORM_ANDROID
	return false;
#endif
}

bool FileUtility::createFolder(const string& path)
{
	// 如果目录已经存在,则返回true
	if (isFileExist(path))
	{
		return true;
	}
	// 如果文件不存在则检查父目录是否存在
	if (!isFileExist(path))
	{
		// 如果有上一级目录,并且上一级目录不存在,则先创建上一级目录
		string parentDir = StringUtility::getFilePath(path);
		if (parentDir != path)
		{
			createFolder(parentDir);
		}
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

bool FileUtility::writeFile(string filePath, const string& text, bool append)
{
	return writeFile(filePath, text.c_str(), text.length(), append);
}

bool FileUtility::writeFile(string filePath, const char* buffer, int length, bool append)
{
#ifdef LOAD_FROM_ASSETMANAGER
	return false;
#endif
	// 检查参数
	if (length < 0 || buffer == NULL)
	{
		//LOG_ERROR("file length error! can not write file! file path : %s", filePath.c_str());
		return false;
	}
	if (length > 0 && buffer == NULL)
	{
		//LOG_ERROR("buffer is NULL! can not write file! file path : %s", filePath.c_str());
		return false;
	}
	// 检查路径
	StringUtility::rightToLeft(filePath);
	int pos = filePath.find_last_of('/');
	if (pos != -1)
	{
		string dirPath = filePath.substr(0, pos);
		if (!createFolder(dirPath))
		{
			//LOG_ERROR("can not create folder, name : %s", dirPath.c_str());
			return false;
		}
	}
	writeFileSimple(filePath, buffer, length, append);
	return true;
}

bool FileUtility::writeFileSimple(const string& fileName, const char* buffer, int writeCount, bool append)
{
	const char* accesMode = append ? "ab+" : "wb+";
#if RUN_PLATFORM == PLATFORM_WINDOWS
	FILE* pFile = NULL;
	fopen_s(&pFile, fileName.c_str(), accesMode);
#elif RUN_PLATFORM == PLATFORM_LINUX
	FILE* pFile = fopen(filePath.c_str(), accesMode);
#endif
	if (pFile == NULL)
	{
		//LOG_ERROR("can not write file, name : %s", fileName.c_str());
		return false;
	}
	fwrite(buffer, sizeof(char), writeCount, pFile);
	fclose(pFile);
	return true;
}

bool FileUtility::writeFileSimple(const string& fileName, const string& text, bool append)
{
	return writeFileSimple(fileName, text.c_str(), text.length(), append);
}

char* FileUtility::openFile(const string& filePath, int* bufferSize, bool addZero)
{
	FILE* pFile = NULL;
#if RUN_PLATFORM == PLATFORM_WINDOWS
	fopen_s(&pFile, filePath.c_str(), "rb");
#elif RUN_PLATFORM == PLATFORM_LINUX
	pFile = fopen(filePath.c_str(), "rb");
#endif
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

string FileUtility::openTxtFile(const string& filePath)
{
	char* buffer = openFile(filePath, NULL, true);
	if (buffer == NULL)
	{
		return EMPTY_STRING;
	}
	string str(buffer);
	TRACE_DELETE_ARRAY(buffer);
	return str;
}

char* FileUtility::openBinaryFile(const string& filePath, int* bufferSize)
{
	return openFile(filePath, bufferSize, false);
}