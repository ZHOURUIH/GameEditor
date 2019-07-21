#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <map>

using namespace std;

void findFiles(const char * pathName, vector<string>& files, vector<string> patterns)
{
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	CHAR PathBuffer[_MAX_PATH];

	strcpy_s(PathBuffer, _MAX_PATH, pathName);
	if (pathName[strlen(pathName) - 1] != '/')
	{
		strcat_s(PathBuffer, _MAX_PATH, "/*");
	}
	else
	{
		strcat_s(PathBuffer, _MAX_PATH, "*");
	}

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
		string fullname = string(pathName) + "/" + string(FindFileData.cFileName);
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
		}
	}
	::FindClose(hFind);
}

string intToString(int i, int limitLen = 0)
{
	char str[256];
	sprintf_s(str, 256, "%d", i);
	string retString(str);
	int addLen = limitLen - strlen(str);
	if (addLen > 0)
	{
		char addStr[256];
		memset(addStr, 0, 256);
		for (int j = 0; j < addLen; ++j)
		{
			addStr[j] = '0';
		}
		retString = string(addStr) + retString;
	}

	return retString;
}

static void rightToLeft(string& str)
{
	int pathLength = str.length();
	for (int i = 0; i < pathLength; ++i)
	{
		if ('\\' == str[i])
		{
			str[i] = '/';
		}
	}
}

static string getParentDir(string dir)
{
	rightToLeft(dir);
	int pos = dir.find_last_of('/');
	string tempDir = dir;
	if (-1 != pos)
	{
		tempDir = dir.substr(0, pos);
	}
	return tempDir;
}

static string getFileName(string str)
{
	rightToLeft(str);
	int dotPos = str.find_last_of('/');
	if (dotPos != -1)
	{
		return str.substr(dotPos + 1, str.length() - 1);
	}
	return str;
}

static string getFileSuffix(string fileName)
{
	int dotPos = fileName.find_last_of('.');
	if (dotPos != -1)
	{
		return fileName.substr(dotPos, fileName.length() - dotPos);
	}
	return fileName;
}

static int stringToInt(const string& str)
{
	return atoi(str.c_str());
}

static void reorderList(vector<string>& fileList)
{
	map<int, string> reorderFiles;
	int fileCount = fileList.size();
	for (int i = 0; i < fileCount; ++i)
	{
		int lastUnderLine = fileList[i].find_last_of('_');
		int lastDot = fileList[i].find_last_of('.');
		string numberSuffix = fileList[i].substr(lastUnderLine + 1, lastDot - lastUnderLine - 1);
		int index = stringToInt(numberSuffix);
		reorderFiles.insert(make_pair(index, fileList[i]));
	}
	fileList.clear();
	auto iter = reorderFiles.begin();
	auto iterEnd = reorderFiles.end();
	for (; iter != iterEnd; ++iter)
	{
		fileList.push_back(iter->second);
	}
}

void main()
{
	vector<string> fileList;
	vector<string> patterns;
	patterns.push_back(".png");
	findFiles("./", fileList, patterns);
	if (fileList.size() == 0)
	{
		return;
	}
	cout << "1.根据文件夹名自动重命名" << endl;
	cout << "2.根据文件名数字后缀排序命名" << endl;
	cout << "3.手动输入前缀重命名" << endl;
	cout << "请选择:";
	int input = 0;
	cin >> input;
	string preName;
	if (input == 1)
	{
		char moduleFileName[MAX_PATH];
		GetModuleFileNameA(0, moduleFileName, MAX_PATH);
		string curFolder = getFileName(getParentDir(moduleFileName));
		preName = curFolder + "_";
	}
	else if (input == 2)
	{
		string fileName = getFileName(fileList[0]);
		preName = fileName.substr(0, fileName.find_last_of('_') + 1);
		reorderList(fileList);
	}
	else if (input == 3)
	{
		cout << "文件前缀:";
		cin >> preName;
	}
	// 重命名文件
	int fileCount = fileList.size();
	for (int i = 0; i < fileCount; ++i)
	{
		string fileDir = getParentDir(fileList[i]);
		string newName = preName + intToString(i) + getFileSuffix(fileList[i]);
		rename(fileList[i].c_str(), (fileDir + "/" + newName).c_str());
	}
}