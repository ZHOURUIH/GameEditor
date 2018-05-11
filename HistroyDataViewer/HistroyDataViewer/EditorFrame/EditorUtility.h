#ifndef _EDITOR_UTILITY_H_
#define _EDITOR_UTILITY_H_

#include "DataDefine.h"
#include "EditorHeader.h"

class EditorUtility
{
public:
	// 去掉文件名的后缀,返回的是文件名的后缀,不带.号
	static wxString removeSuffix(wxString& fileName)
	{
		int dotPos = fileName.find_last_of('.');
		if (dotPos != -1)
		{
			wxString suffix = fileName.substr(dotPos + 1, fileName.length() - dotPos - 1);
			fileName = fileName.substr(0, dotPos);
			return suffix;
		}
		return "";
	}

	static std::string intToString(int i, int limitLen)
	{
		char str[256];
		sprintf_s(str, 256, "%d", i);
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

	// 判断oriString是否以pattern结尾
	static bool endWith(const std::string& oriString, const std::string& pattern)
	{
		if (oriString.length() < pattern.length())
		{
			return false;
		}
		std::string endString = oriString.substr(oriString.length() - pattern.length(), pattern.length());
		return endString == pattern;
	}
	// 判断oriString是否以pattern开头
	static bool startWith(const std::string& oriString, const std::string& pattern)
	{
		if (oriString.length() < pattern.length())
		{
			return false;
		}
		std::string startString = oriString.substr(0, pattern.length());
		return startString == pattern;
	}

	static float pow_f(float f, int p);

	static std::string floatToString(float f, int precision = 3);

	static bool isFileExist(const std::string& fullPath);

	static bool isIgnoreClass(std::set<std::string>& keywordList, const std::string& name);
	static bool isIgnoreClass(std::vector<std::string>& keywordList, const std::string& name);
	static bool checkNumber(const wxString& value);
};

#endif