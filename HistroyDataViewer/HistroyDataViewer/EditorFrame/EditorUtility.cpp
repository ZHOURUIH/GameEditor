#include "EditorUtility.h"

float EditorUtility::pow_f(float f, int p)
{
	float ret = 1.0f;
	while (p)
	{
		ret *= f;
		--p;
	}
	return ret;
}

std::string EditorUtility::floatToString(float f, int precision)
{
	f = f * pow_f(10.0f, precision) + 0.5f;
	int MAX_INT = 0x7FFFFFFF;
	if (f > (float)MAX_INT)
	{
		char str[256];
		sprintf_s(str, 256, "%f", f);
		return std::string(str);
	}
	
	int intValue = (int)f;

	char str[256];
	sprintf_s(str, 256, "%d", intValue);
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

bool isFileExist(std::string fullPath)
{
	int ret = _access(fullPath.c_str(), 0);
	return ret == 0;
}


bool EditorUtility::isIgnoreClass(std::set<std::string>& keywordList, const std::string& name)
{
	std::set<std::string>::iterator iter = keywordList.begin();
	std::set<std::string>::iterator iterEnd = keywordList.end();
	for (; iter != iterEnd; ++iter)
	{
		if (strstr(name.c_str(), iter->c_str()) != NULL)
		{
			return true;
		}
	}
	return false;
}

bool EditorUtility::isIgnoreClass(std::vector<std::string>& keywordList, const std::string& name)
{
	std::vector<std::string>::iterator iter = keywordList.begin();
	std::vector<std::string>::iterator iterEnd = keywordList.end();
	for (; iter != iterEnd; ++iter)
	{
		if (strstr(name.c_str(), iter->c_str()) != NULL)
		{
			return true;
		}
	}
	return false;
}

bool EditorUtility::checkNumber(const wxString& value)
{
	int strLength = value.length();
	for (int i = 0; i < strLength; ++i)
	{
		if (value[i] < '0' || value[i] > '9')
		{
			return false;
		}
	}
	return true;
}