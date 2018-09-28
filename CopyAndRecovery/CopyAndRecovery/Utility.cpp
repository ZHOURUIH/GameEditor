#include "Utility.h"

#include <iostream>
#include <stdlib.h>
#if RUN_PLATFORM == PLATFORM_ANDROID
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#elif RUN_PLATFORM == PLATFORM_WINDOWS
#include <windows.h>
#include <io.h>
#include <direct.h>
#endif
#include "txMemeryTrace.h"

#if RUN_PLATFORM == PLATFORM_ANDROID
//判断是否为目录
bool txUtility::IsDirectory(char *pszName)
{
	struct stat S_stat;
	//取得文件状态
	if (lstat(pszName, &S_stat) < 0)
	{
		return false;
	}
	//判断文件是否为文件夹
	return S_ISDIR(S_stat.st_mode);
}

void txUtility::findFiles(const char * pathName, std::vector<std::string>& files, std::vector<std::string> patterns)
{
#ifdef LOAD_FROM_ASSETMANAGER
	std::vector<std::string> fileName = ASS_getFileList((char*)pathName);
	int fileCount = fileName.size();
	for (int i = 0; i < fileCount; ++i)
	{
		std::string fullName = fileName[i];
		// 判断是否符合后缀过滤
		int patternCount = patterns.size();
		for (int j = 0; j < patternCount; ++j)
		{
			if (fullName.length() >= patterns[j].length() && fullName.substr(fullName.length() - patterns[j].length(), patterns[j].length()) == patterns[j])
			{
				files.push_back(fullName);
			}
		}
	}

#else
	int iRet = 0;
	struct dirent *pDirent;
	DIR* pDir = opendir(pathName);
	if (pDir == NULL)
	{
		return;
	}
	while ((pDirent = readdir(pDir)) != NULL)
	{
		//如果是.或者..跳过
		if (std::string(pDirent->d_name) == "." || std::string(pDirent->d_name) == "..")
		{
			continue;
		}
		//判断是否为文件夹
		char szTmpPath[1024];
		memset(szTmpPath, 0, 1024);
#ifdef USE_SAFE_API
		sprintf_s(szTmpPath, 1024, "%s/%s", pathName, pDirent->d_name);
#else
		sprintf(szTmpPath, "%s/%s", pathName, pDirent->d_name);
#endif

		if (IsDirectory(szTmpPath))
		{
			//如果是文件夹则进行递归
			findFiles(szTmpPath, files, patterns);
		}
		else
		{
			std::string fullName = std::string(szTmpPath);
			int patternCount = patterns.size();
			for (int i = 0; i < patternCount; ++i)
			{
				if (fullName.length() >= patterns[i].length() && fullName.substr(fullName.length() - patterns[i].length(), patterns[i].length()) == patterns[i])
				{
					files.push_back(fullName);
				}
			}
		}
	}
	closedir(pDir);
#endif
}

#elif RUN_PLATFORM == PLATFORM_WINDOWS
void Utility::findFiles(const char * pathName, std::vector<std::string>& files, std::vector<std::string> patterns)
{
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	CHAR PathBuffer[_MAX_PATH];

	strcpy_s(PathBuffer, _MAX_PATH, pathName);
	if (pathName[strlen(pathName) - 1] != '/')
	{
		strcat_s(PathBuffer, _MAX_PATH, "/");
	}
	strcat_s(PathBuffer, _MAX_PATH, "*");
	hFind = FindFirstFileA(PathBuffer, &FindFileData);
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
		std::string fullname = std::string(pathName) + "/" + std::string(FindFileData.cFileName);
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			findFiles(fullname.c_str(), files, patterns);
		}
		else
		{
			int patternCount = patterns.size();
			for (int i = 0; i < patternCount; ++i)
			{
				if (fullname.length() >= patterns[i].length() && fullname.substr(fullname.length() - patterns[i].length(), patterns[i].length()) == patterns[i])
				{
					files.push_back(fullname);
				}
			}
			if (0 == patternCount)
			{
				files.push_back(fullname);
			}
		}
	}
	::FindClose(hFind);
}

bool Utility::deleteEmptyFolder(const std::string& path, std::vector<std::string>& emptyFolder)
{
	const char * pathName = path.c_str();
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	CHAR PathBuffer[_MAX_PATH];

	strcpy_s(PathBuffer, _MAX_PATH, pathName);
	if (pathName[strlen(pathName) - 1] != '/')
	{
		strcat_s(PathBuffer, _MAX_PATH, "/");
	}
	strcat_s(PathBuffer, _MAX_PATH, "*");
	hFind = FindFirstFileA(PathBuffer, &FindFileData);
	// 如果找不到文件夹就直接返回
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return true;
	}
	bool empty = true;
	while (::FindNextFileA(hFind, &FindFileData))
	{
		// 过滤.和..
		if (strcmp(FindFileData.cFileName, ".") == 0
			|| strcmp(FindFileData.cFileName, "..") == 0)
		{
			continue;
		}
		// 构造完整路径
		std::string fullname = std::string(pathName) + "/" + std::string(FindFileData.cFileName);
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			empty = deleteEmptyFolder(fullname, emptyFolder) && empty;
		}
		else
		{
			empty = false;
		}
	}
	if (empty)
	{
		emptyFolder.push_back(path);
	}
	::FindClose(hFind);
	return empty;
}
void Utility::deleteFolder(const std::string& path, bool deleteFolderSelf)
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
			deleteFolder(fullname, true);
		}
		// 如果是文件,则直接删除文件
		else
		{
			deleteFile(fullname);
		}
	}
	::FindClose(hFind);
	// 删除文件夹自身
	if (deleteFolderSelf)
	{
		BOOL ret = RemoveDirectoryA(path.c_str());
	}
}

bool Utility::deleteFile(const std::string& fullname)
{
	BOOL ret = DeleteFileA(fullname.c_str());
	if (ret != TRUE)
	{
		int error = GetLastError();
		std::cout << "error code : " << error << std::endl;
	}
	return ret == TRUE;
}

#endif

bool Utility::isFolderEmpty(const std::string& path)
{
	const char * pathName = path.c_str();
	CHAR PathBuffer[_MAX_PATH];

	strcpy_s(PathBuffer, _MAX_PATH, pathName);
	if (pathName[strlen(pathName) - 1] != '/')
	{
		strcat_s(PathBuffer, _MAX_PATH, "/");
	}
	strcat_s(PathBuffer, _MAX_PATH, "*");
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = FindFirstFileA(PathBuffer, &FindFileData);
	// 如果找不到文件夹就直接返回
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return true;
	}
	bool empty = true;
	while (::FindNextFileA(hFind, &FindFileData))
	{
		// 过滤.和..
		if (strcmp(FindFileData.cFileName, ".") == 0
			|| strcmp(FindFileData.cFileName, "..") == 0)
		{
			continue;
		}
		// 构造完整路径
		std::string fullname = std::string(pathName) + "/" + std::string(FindFileData.cFileName);
		// 是文件夹,则递归继续查找
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			empty = isFolderEmpty(fullname) && empty;
		}
		// 有文件则直接判断为非空文件夹
		else
		{
			empty = false;
		}
		if (!empty)
		{
			break;
		}
	}
	::FindClose(hFind);
	return empty;
}

bool Utility::copyFile(const std::string& sourceFile, const std::string& destFile, bool overWrite)
{
	//std::cout << "copy : " << sourceFile << " to :" << destFile << std::endl;
	BOOL ret = CopyFileA(sourceFile.c_str(), destFile.c_str(), !overWrite);
	if (ret != TRUE)
	{
		int error = GetLastError();
		std::cout << "error code : " << error << std::endl;
	}
	return ret == TRUE;
}

bool Utility::moveFile(const std::string& sourceFile, const std::string& destFile)
{
	//std::cout << "move : " << sourceFile << " to : " << destFile << std::endl;
	BOOL ret = MoveFileA(sourceFile.c_str(), destFile.c_str());
	if (ret != TRUE)
	{
		int error = GetLastError();
		std::cout << "error code : " << error << std::endl;
	}
	return ret == TRUE;
}

bool Utility::createFolder(const std::string& path)
{
	std::string parenDir = getParentDir(path);
	if (parenDir != path)
	{
		createFolder(parenDir);
	}

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

bool Utility::writeFile(std::string filePath, int length, const char* buffer)
{
	if (length <= 0 || NULL == buffer)
	{
		std::cout << "error : file length is 0 or buffer is NULL! can not write file! file path : " << filePath << std::endl;
		return false;
	}

	Utility::rightToLeft(filePath);

#ifdef LOAD_FROM_ASSETMANAGER
	return false;
#else
	int pos = filePath.find_last_of('/');
	if (-1 != pos)
	{
		std::string dirPath = filePath.substr(0, pos);
		if (!createFolder(dirPath))
		{
			std::cout << "error : can not create folder, name : " << filePath << std::endl;
			return false;
		}
	}
	else
	{
		if (!createFolder(filePath))
		{
			std::cout << "error : can not create folder, name : " << filePath << std::endl;
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
		//ENGINE_ERROR("error : can not write file, name : %s", filePath.c_str());
		std::cout << "error : can not write file, name ", filePath.c_str();
		return false;
	}

	fwrite(buffer, sizeof(char), length, pFile);
	fclose(pFile);
#endif

	return true;
}

bool Utility::isFileExist(const std::string& fullPath)
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

std::string Utility::intToString(int i, int limitLen)
{
	char str[256];
#ifdef _USE_SAFE_API
	sprintf_s(str, 256, "%d", i);
#else
	sprintf(str, "%d", i);
#endif
	std::string retString(str);
	int addLen = limitLen - strlen(str);
	if (addLen > 0)
	{
		char addStr[256];
		memset(addStr, 0, 256);
		for (int j = 0; j < addLen; ++j)
		{
			addStr[j] = '0';
		}
		retString = std::string(addStr) + retString;
	}

	return retString;
}

float Utility::pow_f(float f, int p)
{
	float ret = 1.0f;
	while (p)
	{
		ret *= f;
		--p;
	}
	return ret;
}

std::string Utility::floatToString(float f, int precision)
{
	f = f * pow_f(10.0f, precision) + 0.5f;
	int MAX_INT = 0x7FFFFFFF;
	if (f > (float)MAX_INT)
	{
		char str[256];
#ifdef _USE_SAFE_API
		sprintf_s(str, 256, "%f", f);
#else
		sprintf(str, "%f", f);
#endif
		return std::string(str);
	}

	int intValue = (int)f;

	char str[256];
#ifdef _USE_SAFE_API
	sprintf_s(str, 256, "%d", intValue);
#else
	sprintf(str, "%d", intValue);
#endif
	if (precision == 0)
	{
		return std::string(str);
	}
	else
	{
		int length = strlen(str);
		if (length <= precision)
		{
			char newStr[256];
			memset(newStr, 0, 256 * sizeof(char));
			for (int i = 0; i < precision - length + 1; ++i)
			{
				newStr[i] = '0';
			}
			memcpy(newStr + precision - length + 1, str, length);

			char intStr[256];
			memset(intStr, 0, 256 * sizeof(char));
			char decimalStr[256];
			memset(decimalStr, 0, 256 * sizeof(char));
			memcpy(intStr, newStr, strlen(newStr) - precision);
			memcpy(decimalStr, newStr + strlen(newStr) - precision, precision);
			return std::string(intStr) + "." + std::string(decimalStr);
		}
		else
		{
			char intStr[256];
			memset(intStr, 0, 256 * sizeof(char));
			char decimalStr[256];
			memset(decimalStr, 0, 256 * sizeof(char));
			memcpy(intStr, str, strlen(str) - precision);
			memcpy(decimalStr, str + strlen(str) - precision, precision);
			return std::string(intStr) + "." + std::string(decimalStr);
		}
	}
}

void Utility::split(std::string str, const char* deli, std::vector<std::string>& vec)
{
	while (true)
	{
		int devidePos = str.find_first_of(deli);
		if (devidePos == -1)
		{
			vec.push_back(str);
			break;
		}
		else
		{
			std::string curString = str.substr(0, devidePos);
			vec.push_back(curString);
			str = str.substr(devidePos + strlen(deli), str.length() - devidePos - strlen(deli));
		}
	}
}

char* Utility::openFile(const std::string& filePath, int* bufferSize, const bool& addZero)
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

std::string Utility::openTxtFile(const std::string& filePath)
{
	char* buffer = openFile(filePath, NULL, true);
	if (buffer == NULL)
	{
		return "";
	}
	std::string str(buffer);
	TRACE_DELETE_ARRAY(buffer);
	return str;
}

char* Utility::openBinaryFile(const std::string& filePath, int* bufferSize)
{
	return openFile(filePath, bufferSize, false);
}