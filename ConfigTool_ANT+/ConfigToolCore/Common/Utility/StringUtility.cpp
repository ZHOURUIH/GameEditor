#include "StringUtility.h"
#include "ToolCoreUtility.h"
#include "ToolCoreLog.h"
#include "txMemoryTrace.h"

std::string StringUtility::removeSuffix(std::string str)
{
	int dotPos = str.find_last_of('.');
	if (dotPos != -1)
	{
		return str.substr(0, dotPos);
	}
	return str;
}

void StringUtility::removeLastComma(std::string& stream)
{
	int streamSize = stream.length();
	for (int i = 0; i < streamSize; ++i)
	{
		if (stream[streamSize - i - 1] == ',')
		{
			stream.erase(streamSize - i - 1, 1);
			break;
		}
	}
}

std::string StringUtility::getFileName(std::string str)
{
	rightToLeft(str);
	int dotPos = str.find_last_of('/');
	if (dotPos != -1)
	{
		return str.substr(dotPos + 1, str.length() - 1);
	}
	return str;
}

std::string StringUtility::getFileNameNoSuffix(std::string str)
{
	rightToLeft(str);
	int namePos = str.find_last_of('/');
	int dotPos = str.find_last_of('.');
	if (dotPos != -1)
	{
		return str.substr(namePos + 1, dotPos - namePos - 1);
	}

	return str;
}

std::string StringUtility::getParentDir(std::string dir)
{
	rightToLeft(dir);
	int pos = dir.find_last_of('/');
	std::string tempDir = dir;
	if (-1 != pos)
	{
		tempDir = dir.substr(0, pos);
	}

	return tempDir;
}

std::string StringUtility::getFileSuffix(std::string fileName)
{
	int dotPos = fileName.find_last_of('.');
	if (dotPos != -1)
	{
		return fileName.substr(dotPos + 1, fileName.length() - dotPos - 1);
	}
	return fileName;
}

void StringUtility::split(std::string str, const char* deli, txVector<std::string>& vec, bool removeEmpty)
{
	while (true)
	{
		int devidePos = str.find_first_of(deli);
		if (devidePos == -1)
		{
			if (!removeEmpty || str != "")
			{
				vec.push_back(str);
			}
			break;
		}
		else
		{
			std::string curString = str.substr(0, devidePos);
			if (!removeEmpty || curString != "")
			{
				vec.push_back(curString);
			}
			str = str.substr(devidePos + strlen(deli), str.length() - devidePos - strlen(deli));
		}
	}
}

std::string StringUtility::strReplace(const std::string& str, const int& begin, const int& end, const std::string& reStr)
{
	std::string sub1 = str.substr(0, begin);
	std::string sub2 = str.substr(end, str.length() - end);
	return sub1 + reStr + sub2;
}

std::string StringUtility::strReplaceAll(std::string str, const std::string& key, const std::string& newWords)
{
	int startPos = 0;
	while (true)
	{
		int pos = strFind(str, key, startPos);
		if (pos == -1)
		{
			break;
		}
		str = strReplace(str, pos, pos + key.length(), newWords);
		startPos = pos + newWords.length();
	}
	return str;
}

std::string StringUtility::bufferToString(char* buffer, const int& bufferSize)
{
	char* tempBuffer = TRACE_NEW_ARRAY(char, bufferSize + 1, tempBuffer);
	memset(tempBuffer, 0, bufferSize + 1);
	memcpy(tempBuffer, buffer, bufferSize);
	std::string str(tempBuffer);
	TRACE_DELETE_ARRAY(tempBuffer);
	return str;
}

std::string StringUtility::intToString(int i, int limitLen)
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

int StringUtility::stringToInt(std::string str)
{
	str = checkNumberString(str);
	if (str == "")
	{
		return 0;
	}
	return atoi(str.c_str()); 
}

std::string StringUtility::checkString(const std::string& str, const std::string& valid)
{
	std::string newString = "";
	int validCount = valid.length();
	int oldStrLen = str.length();
	for (int i = 0; i < oldStrLen; ++i)
	{
		bool keep = false;
		for (int j = 0; j < validCount; ++j)
		{
			if (str[i] == valid[j])
			{
				keep = true;
				break;
			}
		}
		if (keep)
		{
			newString += str[i];
		}
	}
	return newString;
}

std::string StringUtility::checkNumberString(const std::string& str, const std::string& valid)
{
	return checkString(str, "0123456789." + valid);
}

float StringUtility::pow_f(float f, int p)
{
	float ret = 1.0f;
	while (p)
	{
		ret *= f;
		--p;
	}
	return ret;
}

std::string StringUtility::floatToString(float f, int precision)
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

float StringUtility::stringToFloat(std::string str)
{ 
	str = checkNumberString(str);
	if (str == "")
	{
		return 0.0f;
	}
	return (float)atof(str.c_str()); 
}

bool StringUtility::endWith(std::string oriString, std::string pattern)
{
	if (oriString.length() < pattern.length())
	{
		return false;
	}
	std::string endString = oriString.substr(oriString.length() - pattern.length(), pattern.length());
	return endString == pattern;
}

bool StringUtility::startWith(std::string oriString, std::string pattern)
{
	if (oriString.length() < pattern.length())
	{
		return false;
	}
	std::string startString = oriString.substr(0, pattern.length());
	return startString == pattern;
}

std::wstring StringUtility::ANSIToUnicode(const std::string& str)
{
	int len = str.length();
	int unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	wchar_t* pUnicode = TRACE_NEW_ARRAY(wchar_t, unicodeLen + 1, pUnicode);
	memset(pUnicode, 0, (unicodeLen + 1)*sizeof(wchar_t));
	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, (LPWSTR)pUnicode, unicodeLen);
	std::wstring rt = (wchar_t*)pUnicode;
	TRACE_DELETE_ARRAY(pUnicode);
	return rt;
}

std::string StringUtility::UnicodeToANSI(const std::wstring& str)
{
	int iTextLen = WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
	char* pElementText = TRACE_NEW_ARRAY(char, iTextLen + 1, pElementText);
	memset((void*)pElementText, 0, sizeof(char)* (iTextLen + 1));
	::WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, pElementText, iTextLen, NULL, NULL);
	std::string strText = pElementText;
	TRACE_DELETE_ARRAY(pElementText);
	return strText;
}
std::string StringUtility::UnicodeToUTF8(const std::wstring& str)
{
	// wide char to multi char
	int iTextLen = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
	char* pElementText = TRACE_NEW_ARRAY(char, iTextLen + 1, pElementText);
	memset((void*)pElementText, 0, sizeof(char)* (iTextLen + 1));
	::WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, pElementText, iTextLen, NULL, NULL);
	std::string strText = pElementText;
	TRACE_DELETE_ARRAY(pElementText);
	return strText;
}

std::wstring StringUtility::UTF8ToUnicode(const std::string& str)
{
	int len = str.length();
	int unicodeLen = ::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	wchar_t* pUnicode = TRACE_NEW_ARRAY(wchar_t, unicodeLen + 1, pUnicode);
	memset(pUnicode, 0, (unicodeLen + 1)*sizeof(wchar_t));
	::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, (LPWSTR)pUnicode, unicodeLen);
	std::wstring rt = (wchar_t*)pUnicode;
	TRACE_DELETE_ARRAY(pUnicode);
	return rt;
}

std::string StringUtility::ANSIToUTF8(const std::string& str)
{
	std::wstring unicodeStr = ANSIToUnicode(str);
	std::string utf8Str = UnicodeToUTF8(unicodeStr);
	return utf8Str;
}

std::string StringUtility::UTF8ToANSI(const std::string& str)
{
	std::wstring unicodeStr = UTF8ToUnicode(str);
	std::string ansiStr = UnicodeToANSI(unicodeStr);
	return ansiStr;
}

void StringUtility::rightToLeft(std::string& str)
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

void StringUtility::leftToRight(std::string& str)
{
	int pathLength = str.length();
	for (int i = 0; i < pathLength; ++i)
	{
		if ('/' == str[i])
		{
			str[i] = '\\';
		}
	}
}

int StringUtility::strFind(std::string res, std::string sub, int startPose, bool fromStart)
{
	int searchLength = res.length() - sub.length();
	if (fromStart)
	{
		int j = 0;
		for (int i = startPose; i < searchLength; ++i)
		{
			for (j = 0; j != sub.length(); ++j)
			{
				if (res[i + j] != sub[j])
				{
					break;
				}
			}

			if (j == sub.length())
			{
				return i;
			}
		}
	}
	else
	{
		int j = 0;
		for (int i = searchLength; i != startPose; --i)
		{
			for (j = 0; j != sub.length(); ++j)
			{
				if (res[i + j] != sub[j])
				{
					break;
				}
			}

			if (j == sub.length())
			{
				return i;
			}
		}
	}

	return -1;
}

bool StringUtility::findSubstr(std::string res, std::string dst, bool sensitive, int* pos, int startPose, bool firstOrLast)
{
	// 如果不区分大小写
	if (!sensitive)
	{
		// 全转换为小写
		strToLower(res);
		strToLower(dst);
	}
	int posFind = strFind(res, dst, startPose, firstOrLast);

	if (NULL != pos)
	{
		*pos = posFind;
	}
	if (-1 != posFind)
	{
		return true;
	}

	return false;
}

txVector<std::string> StringUtility::findSubstr(txVector<std::string>& res, std::string dst, bool sensitive)
{
	txVector<std::string> retList;
	int listSize = res.size();
	for (int i = 0; i < listSize; ++i)
	{
		if (findSubstr(res[i], dst, sensitive))
		{
			retList.push_back(res[i]);
		}
	}
	return retList;
}

void StringUtility::strToLower(std::string& str)
{
	std::string::size_type size = str.length();
	for (std::string::size_type i = 0; i != size; ++i)
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
		{
			str[i] += 'a' - 'A';
		}
	}
}

void StringUtility::strToUpper(std::string& str)
{
	std::string::size_type size = str.length();
	for (std::string::size_type i = 0; i != size; ++i)
	{
		if (str[i] >= 'a' && str[i] <= 'z')
		{
			str[i] -= 'a' - 'A';
		}
	}
}

std::string StringUtility::charToHex(unsigned char byte, const bool& upper)
{
	char byteHex[3] = { 0 };
	char cA = upper ? 'A' : 'a';
	unsigned char highBit = byte >> 4;
	// 高字节的十六进制
	if (highBit < (unsigned char)10)
	{
		byteHex[0] = '0' + highBit;
	}
	else
	{
		byteHex[0] = cA + highBit - 10;
	}
	// 低字节的十六进制
	unsigned char lowBit = byte & 0x0F;
	if (lowBit < (unsigned char)10)
	{
		byteHex[1] = '0' + lowBit;
	}
	else
	{
		byteHex[1] = cA + lowBit - 10;
	}
	return byteHex;
}

unsigned char StringUtility::hexStringToChar(const std::string& str)
{
	unsigned char highBit = 0;
	unsigned char lowBit = 0;
	char highBitChar = str[0];
	char lowBitChar = str[1];
	if (highBitChar >= 'A' && highBitChar <= 'F')
	{
		highBit = 10 + highBitChar - 'A';
	}
	else
	{
		highBit = highBitChar - '0';
	}
	if (lowBitChar >= 'A' && lowBitChar <= 'F')
	{
		lowBit = 10 + lowBitChar - 'A';
	}
	else
	{
		lowBit = lowBitChar - '0';
	}
	return highBit << 4 | lowBit;
}

std::string StringUtility::charArrayToHexString(unsigned char* data, const int& dataCount, const bool& addSpace, const bool& upper)
{
	int oneLength = addSpace ? 3 : 2;
	int showCount = dataCount * oneLength + 1;
	char* byteData = TRACE_NEW_ARRAY(char, showCount, byteData);
	memset(byteData, 0, showCount);
	for (int j = 0; j < dataCount; ++j)
	{
		unsigned char curByte = data[j];
		std::string byteStr = charToHex(curByte, upper);
		byteData[j * oneLength + 0] = byteStr[0];
		byteData[j * oneLength + 1] = byteStr[1];
		if (oneLength >= 3)
		{
			byteData[j * oneLength + 2] = ' ';
		}
	}
	std::string str(byteData);
	TRACE_DELETE_ARRAY(byteData);
	return str;
}

void StringUtility::hexStringToCharArray(const std::string& str, unsigned char*& data, int& dataCount)
{
	std::string newStr = StringUtility::strReplaceAll(str, " ", "");
	dataCount = newStr.length() / 2;
	data = TRACE_NEW_ARRAY(unsigned char, dataCount, data);
	for (int i = 0; i < dataCount; ++i)
	{
		data[i] = hexStringToChar(newStr.substr(i * 2, 2));
	}
}