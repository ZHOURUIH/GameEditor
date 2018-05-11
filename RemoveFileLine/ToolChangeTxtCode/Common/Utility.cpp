#include "Utility.h"

#include <windows.h>
#include <io.h>
#include <direct.h>

void Utility::findFiles(const char * pathName, std::vector<std::string>& files, std::vector<std::string> patterns, bool recursive)
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

CODE_TYPE Utility::getFileCodeType(const char* fileBuffer, int length)
{
	if (NULL != fileBuffer)
	{
		unsigned char unicodeContext[] = { 0xFF, 0xFE };
		unsigned char utf8Context[] = { 0xEF, 0xBB, 0xBF };

		
		if (length >= 3)
		{
			if (0 == memcmp(fileBuffer, utf8Context, 3))
			{
				return CT_UTF8;
			}
			else if (0 == memcmp(fileBuffer, unicodeContext, 2))
			{
				return CT_UNICODE;
			}
			else
			{
				return CT_ANSI;
			}
		}
		else if (length == 2)
		{
			if (0 == memcmp(fileBuffer, unicodeContext, 2))
			{
				return CT_UNICODE;
			}
			else
			{
				return CT_ANSI;
			}
		}
		else if (length <= 1)
		{
			return CT_ANSI;
		}

	}
	return CT_MAX;
}


bool Utility::createFolder(std::string path)
{
	// 如果有上一级目录,并且上一级目录不存在,则先创建上一级目录
	std::string parentDir = getParentDir(path);
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

bool Utility::isFileExist(std::string fullPath)
{
	int ret = _access(fullPath.c_str(), 0);

	return ret == 0;
}

void Utility::readFile(std::string filePath, char* &bufferOut, int& fileLengthOut)
{
	rightToLeft(filePath);

	FILE* file = NULL;
	fopen_s(&file, filePath.c_str(), "rb");
	if (NULL == file)
	{
		return;
	}
	fseek(file, 0, SEEK_END);
	int fileLength = ftell(file);
	// 把指针定位到开头
	rewind(file);

	char* buffer = new char[fileLength + 1];
	fread(buffer, sizeof(char), fileLength, file);
	buffer[fileLength] = 0;
	bufferOut = buffer;
	fileLengthOut = fileLength;
	fclose(file);
}

bool Utility::writeFile(std::string filePath, int length, const char* buffer)
{
	if (length <= 0 || NULL == buffer)
	{
		return false;
	}

	rightToLeft(filePath);

	int pos = filePath.find_last_of('/');
	if (-1 != pos)
	{
		std::string dirPath = filePath.substr(0, pos);
		if (!createFolder(dirPath))
		{
			return false;
		}
	}
	else
	{
		if (!createFolder(filePath))
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

wchar_t* Utility::ANSIToUnicode(const char* buffer, int& stringLengthOut)
{
	int unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, buffer, -1, NULL, 0);
	wchar_t* pUnicode = new wchar_t[unicodeLen];
	memset(pUnicode, 0, unicodeLen * sizeof(wchar_t));
	::MultiByteToWideChar(CP_ACP, 0, buffer, -1, (LPWSTR)pUnicode, unicodeLen);
	stringLengthOut = unicodeLen - 1;
	return pUnicode;
}

char* Utility::UnicodeToANSI(const wchar_t* buffer, int& stringLengthOut)
{
	int iTextLen = WideCharToMultiByte(CP_ACP, 0, buffer, -1, NULL, 0, NULL, NULL);
	char* pElementText = new char[iTextLen];
	memset((void*)pElementText, 0, sizeof(char) * iTextLen);
	::WideCharToMultiByte(CP_ACP, 0, buffer, -1, pElementText, iTextLen, NULL, NULL);
	stringLengthOut = iTextLen - 1;
	return pElementText;
}
char* Utility::UnicodeToUTF8(const wchar_t* buffer, int& stringLengthOut)
{
	// wide char to multi char
	int iTextLen = WideCharToMultiByte(CP_UTF8, 0, buffer, -1, NULL, 0, NULL, NULL);
	char* pElementText = new char[iTextLen];
	memset((void*)pElementText, 0, sizeof(char) * iTextLen);
	::WideCharToMultiByte(CP_UTF8, 0, buffer, -1, pElementText, iTextLen, NULL, NULL);
	stringLengthOut = iTextLen - 1;
	return pElementText;
}

wchar_t* Utility::UTF8ToUnicode(const char* buffer, int& stringLengthOut)
{
	int unicodeLen = ::MultiByteToWideChar(CP_UTF8, 0, buffer, -1, NULL, 0);
	wchar_t* pUnicode = new wchar_t[unicodeLen];
	memset(pUnicode, 0, unicodeLen * sizeof(wchar_t));
	::MultiByteToWideChar(CP_UTF8, 0, buffer, -1, (LPWSTR)pUnicode, unicodeLen);
	stringLengthOut = unicodeLen - 1;
	return pUnicode;
}

char* Utility::ANSIToUTF8(const char* buffer, int& stringLengthOut)
{
	wchar_t* unicodeBuffer = ANSIToUnicode(buffer, stringLengthOut);
	char* utf8Buffer = UnicodeToUTF8(unicodeBuffer, stringLengthOut);
	delete[] unicodeBuffer;
	return utf8Buffer;
}

char* Utility::UTF8ToANSI(const char* buffer, int& stringLengthOut)
{
	wchar_t* unicodeBuffer = UTF8ToUnicode(buffer, stringLengthOut);
	char* ansiBuffer = UnicodeToANSI(unicodeBuffer, stringLengthOut);
	delete[] unicodeBuffer;
	return ansiBuffer;
}

char* Utility::addUTF8BOM(char* buffer, int strLength, int& byteLengthOut, bool needEndNull /*= false*/)
{
	unsigned char bom[] = { 0xEF, 0xBB, 0xBF };
	int newLength = 0;
	if (needEndNull)
	{
		newLength = strLength + sizeof(bom) + sizeof(char);
	}
	else
	{
		newLength = strLength + sizeof(bom);
	}
	char* newBuffer = new char[newLength];
	memset(newBuffer, 0, newLength * sizeof(char));
	memcpy(newBuffer, bom, sizeof(bom));
	memcpy(newBuffer + sizeof(bom), buffer, strLength * sizeof(char));
	byteLengthOut = newLength * sizeof(char);
	return newBuffer;
}

wchar_t* Utility::addUNICODEBOM(wchar_t* buffer, int strLength, int& byteLengthOut, bool needEndNull /*= false*/)
{
	unsigned char bom[] = { 0xFF, 0xFE };
	int newLength = 0;
	if (needEndNull)
	{
		newLength = strLength + sizeof(bom) / sizeof(wchar_t) + sizeof(char);
	}
	else
	{
		newLength = strLength + sizeof(bom) / sizeof(wchar_t);
	}

	wchar_t* newBuffer = new wchar_t[newLength];
	memset(newBuffer, 0, newLength * sizeof(wchar_t));
	memcpy(newBuffer, bom, sizeof(bom));
	memcpy(newBuffer + sizeof(bom) / sizeof(wchar_t), buffer, strLength * sizeof(wchar_t));
	byteLengthOut = newLength * sizeof(wchar_t);
	return newBuffer; 	
}

char* Utility::deleteUTF8BOM(char* buffer, int strLength, int& byteLengthOut, bool needEndNull/* = false*/)
{
	unsigned char bom[] = { 0xEF, 0xBB, 0xBF };
	int newLength = 0;
	if (needEndNull)
	{
		newLength = strLength - sizeof(bom) / sizeof(char) + sizeof(char);
	}
	else
	{
		newLength = strLength - sizeof(bom) / sizeof(char);
	}
	char* newBuffer = new char[newLength];
	memset(newBuffer, 0, newLength);
	memcpy(newBuffer, buffer + sizeof(bom), strLength * sizeof(char) - sizeof(bom));
	byteLengthOut = newLength* sizeof(char);
	return newBuffer;
}

wchar_t* Utility::deleteUNICODEBOM(char* buffer, int byteLength, int& byteLengthOut, bool needEndNull /*= false*/)
{
	unsigned char bom[] = { 0xFF, 0xFE };
	int newLength = 0;
	if (needEndNull)
	{
		newLength = byteLength - sizeof(bom) + sizeof(wchar_t);
	}
	else
	{
		newLength = byteLength - sizeof(bom);
	}

	wchar_t* newBuffer = (wchar_t*)new char[newLength];
	memset(newBuffer, 0, newLength);
	memcpy(newBuffer, buffer + sizeof(bom), byteLength - sizeof(bom));
	byteLengthOut = newLength;
	return newBuffer;
}

void Utility::split(std::string str, const char* deli, std::vector<std::string>* vec)
{
	while (true)
	{
		int devidePos = str.find_first_of(deli);
		if (devidePos == -1)
		{
			vec->push_back(str);
			break;
		}
		else
		{
			std::string curString = str.substr(0, devidePos);
			vec->push_back(curString);
			str = str.substr(devidePos + strlen(deli), str.length() - devidePos - strlen(deli));
		}
	}
}
