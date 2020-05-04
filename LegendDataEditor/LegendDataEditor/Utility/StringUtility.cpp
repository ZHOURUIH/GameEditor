#include "Utility.h"

string StringUtility::removeSuffix(const string& str)
{
	int dotPos = str.find_last_of('.');
	if (dotPos != -1)
	{
		return str.substr(0, dotPos);
	}
	return str;
}

void StringUtility::removeLast(string& stream, const string& key)
{
	int lastPos = (int)stream.find_last_of(key);
	if (lastPos != -1)
	{
		stream.erase(lastPos, key.length());
	}
}

void StringUtility::removeLastComma(string& stream)
{
	removeLast(stream, ",");
}

string StringUtility::getFolderName(string str)
{
	string path = getFilePath(str);
	string folder = getFileName(path);
	return folder;
}

string StringUtility::getFileName(string str)
{
	rightToLeft(str);
	int dotPos = str.find_last_of('/');
	if (dotPos != -1)
	{
		return str.substr(dotPos + 1, str.length() - 1);
	}
	return str;
}

string StringUtility::getFileNameNoSuffix(string str, bool removePath)
{
	rightToLeft(str);
	// 已经移除路径后,从头开始查找后缀名,未移除路径时,从文件名处开始查找后缀名
	int namePos = str.find_last_of('/');
	if (removePath)
	{
		str = str.substr(namePos + 1, str.length() - namePos - 1);
		namePos = 0;
	}
	int dotPos = str.find_first_of('.', namePos);
	if (dotPos != -1)
	{
		return str.substr(0, dotPos);
	}
	return str;
}

string StringUtility::getFilePath(string dir)
{
	rightToLeft(dir);
	// 如果最后一个字符是斜杠,则需要将最后的斜杠去掉
	if (dir[dir.length() - 1] == '/')
	{
		dir.erase(dir.length() - 1, 1);
	}
	int pos = dir.find_last_of('/');
	string tempDir = dir;
	if (-1 != pos)
	{
		tempDir = dir.substr(0, pos);
	}
	return tempDir;
}

string StringUtility::getFileSuffix(const string& fileName, bool includeDot)
{
	int dotPos = fileName.find_last_of('.');
	if (dotPos != -1)
	{
		int startPos = dotPos;
		if (!includeDot)
		{
			startPos += 1;
		}
		return fileName.substr(startPos, fileName.length() - startPos);
	}
	return fileName;
}

int StringUtility::getLastNotNumberPos(const string& str)
{
	int strLen = str.length();
	for (int i = 0; i < strLen; ++i)
	{
		if (str[strLen - i - 1] > '9' || str[strLen - i - 1] < '0')
		{
			return strLen - i - 1;
		}
	}
	return -1;
}

string StringUtility::getNotNumberSubString(const string& str)
{
	int notNumPos = getLastNotNumberPos(str);
	return str.substr(0, notNumPos + 1);
}

int StringUtility::getLastNumber(const string& str)
{
	int lastPos = getLastNotNumberPos(str);
	if (lastPos == -1)
	{
		return -1;
	}
	string numStr = str.substr(lastPos + 1, str.length() - lastPos - 1);
	if (numStr == EMPTY_STRING)
	{
		return 0;
	}
	return stringToInt(numStr);
}

void StringUtility::split(string str, const string& deli, txVector<string>& vec, bool removeEmpty)
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
			string curString = str.substr(0, devidePos);
			if (!removeEmpty || curString != "")
			{
				vec.push_back(curString);
			}
			str = str.substr(devidePos + deli.length(), str.length() - devidePos - deli.length());
		}
	}
}

string StringUtility::strReplace(const string& str, int begin, int end, const string& reStr)
{
	string sub1 = str.substr(0, begin);
	string sub2 = str.substr(end, str.length() - end);
	return sub1 + reStr + sub2;
}

void StringUtility::replaceAll(string& str, const char* string_to_replace, const char* new_string)
{
	int index = str.find(string_to_replace);
	while (index != string::npos)
	{
		str.replace(index, strlen(string_to_replace), new_string);
		index = str.find(string_to_replace, index + strlen(new_string));
	}
}

string StringUtility::intToString(int i, int limitLen)
{
	char str[256];
	SPRINTF(str, 256, "%d", i);
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

string StringUtility::intArrayToString(const txVector<int>& intArray, int limitLen, const string& key)
{
	string str;
	int count = intArray.size();
	for (int i = 0; i < count; ++i)
	{
		str += intToString(intArray[i]) + key;
	}
	removeLast(str, key);
	return str;
}

void StringUtility::stringToVector2i(const string& str, Vector2i& vec2, const string& key)
{
	txVector<string> strList;
	split(str, key, strList);
	if (strList.size() >= 2)
	{
		vec2.x = stringToInt(strList[0]);
		vec2.y = stringToInt(strList[1]);
	}
}

void StringUtility::stringToIntArray(const string& str, txVector<int>& intArray, const string& key)
{
	txVector<string> strList;
	split(str, key, strList);
	int count = strList.size();
	for (int i = 0; i < count; ++i)
	{
		intArray.push_back(stringToInt(strList[i]));
	}
}

string StringUtility::floatToString(float f, int precision, bool removeZero)
{
	string retString;
	for (int temp = 0; temp < 1; ++temp)
	{
		if (!MathUtility::isFloatZero(f))
		{
			f = f * MathUtility::powerFloat(10.0f, precision) + f / std::abs(f) * 0.5f;
		}
		int MAX_INT = 0x7FFFFFFF;
		if (f > (float)MAX_INT)
		{
			char str[256];
			SPRINTF(str, 256, "%f", f);
			retString = str;
			break;
		}

		char str[256];
		SPRINTF(str, 256, "%d", (int)f);
		if (precision == 0)
		{
			retString = str;
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
				retString = string(intStr) + "." + string(decimalStr);
			}
			else
			{
				char intStr[256];
				memset(intStr, 0, 256 * sizeof(char));
				char decimalStr[256];
				memset(decimalStr, 0, 256 * sizeof(char));
				memcpy(intStr, str, strlen(str) - precision);
				memcpy(decimalStr, str + strlen(str) - precision, precision);
				retString = string(intStr) + "." + string(decimalStr);
			}
		}
	}
	// 移除末尾无用的0
	if (removeZero)
	{
		int dotPos = retString.find_last_of('.');
		if (dotPos != -1)
		{
			string floatPart = retString.substr(dotPos + 1, retString.length() - dotPos - 1);
			// 找到最后一个不是0的位置,然后将后面的所有0都去掉
			int notZeroPos = floatPart.find_last_not_of('0');
			// 如果小数部分全是0,则将小数点也一起去掉
			if (notZeroPos == -1)
			{
				retString = retString.substr(0, dotPos);
			}
			else if (notZeroPos != (int)floatPart.length() - 1)
			{
				floatPart = floatPart.substr(0, notZeroPos + 1);
				retString = retString.substr(0, dotPos + 1) + floatPart;
			}
		}
	}
	return retString;
}

void StringUtility::stringToFloatArray(const string& str, float*& floatArray, const string& key)
{
	txVector<string> strList;
	split(str, key, strList);
	int count = strList.size();
	floatArray = TRACE_NEW_ARRAY(float, count, floatArray);
	for (int i = 0; i < count; ++i)
	{
		floatArray[i] = stringToFloat(strList[i]);
	}
}

void StringUtility::stringToFloatArray(const string& str, txVector<float>& floatArray, const string& key)
{
	txVector<string> strList;
	split(str, key, strList);
	int count = strList.size();
	for (int i = 0; i < count; ++i)
	{
		floatArray.push_back(stringToFloat(strList[i]));
	}
}

string StringUtility::floatArrayToString(const txVector<float>& floatArray, const string& key)
{
	string str;
	int count = floatArray.size();
	for (int i = 0; i < count; ++i)
	{
		str += floatToString(floatArray[i]) + key;
	}
	removeLast(str, key);
	return str;
}

bool StringUtility::endWith(const string& oriString, const string& pattern, bool sensitive)
{
	if (oriString.length() < pattern.length())
	{
		return false;
	}
	string endString = oriString.substr(oriString.length() - pattern.length(), pattern.length());
	if (sensitive)
	{
		return endString == pattern;
	}
	else
	{
		strToLower(endString);
		string temp = pattern;
		strToLower(temp);
		return endString == temp;
	}
}

bool StringUtility::startWith(const string& oriString, const string& pattern, bool sensitive)
{
	if (oriString.length() < pattern.length())
	{
		return false;
	}
	string startString = oriString.substr(0, pattern.length());
	if (sensitive)
	{
		return startString == pattern;
	}
	else
	{
		strToLower(startString);
		string temp = pattern;
		strToLower(temp);
		return startString == temp;
	}
}

#if RUN_PLATFORM == PLATFORM_WINDOWS
std::wstring StringUtility::ANSIToUnicode(const string& str)
{
	int unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	wchar_t* pUnicode = TRACE_NEW_ARRAY(wchar_t, unicodeLen + 1, pUnicode);
	memset(pUnicode, 0, (unicodeLen + 1)*sizeof(wchar_t));
	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, (LPWSTR)pUnicode, unicodeLen);
	std::wstring rt = (wchar_t*)pUnicode;
	TRACE_DELETE_ARRAY(pUnicode);
	return rt;
}

string StringUtility::UnicodeToANSI(const std::wstring& str)
{
	int iTextLen = WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
	char* pElementText = TRACE_NEW_ARRAY(char, iTextLen + 1, pElementText);
	memset((void*)pElementText, 0, sizeof(char)* (iTextLen + 1));
	::WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, pElementText, iTextLen, NULL, NULL);
	string strText = pElementText;
	TRACE_DELETE_ARRAY(pElementText);
	return strText;
}
string StringUtility::UnicodeToUTF8(const std::wstring& str)
{
	// wide char to multi char
	int iTextLen = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
	char* pElementText = TRACE_NEW_ARRAY(char, iTextLen + 1, pElementText);
	memset((void*)pElementText, 0, sizeof(char)* (iTextLen + 1));
	::WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, pElementText, iTextLen, NULL, NULL);
	string strText = pElementText;
	TRACE_DELETE_ARRAY(pElementText);
	return strText;
}

std::wstring StringUtility::UTF8ToUnicode(const string& str)
{
	int unicodeLen = ::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	wchar_t* pUnicode = TRACE_NEW_ARRAY(wchar_t, unicodeLen + 1, pUnicode);
	memset(pUnicode, 0, (unicodeLen + 1)*sizeof(wchar_t));
	::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, (LPWSTR)pUnicode, unicodeLen);
	std::wstring rt = (wchar_t*)pUnicode;
	TRACE_DELETE_ARRAY(pUnicode);
	return rt;
}

#elif RUN_PLATFORM == PLATFORM_ANDROID
std::wstring StringUtility::ANSIToUnicode(const string& str)
{
	if (str == EMPTY_STRING)
	{
		return L"";
	}

	string oldname = setlocale(LC_ALL, NULL);
	string curName = setlocale(LC_ALL, LC_NAME_zh_CN_GBK);

	int dSize = mbstowcs(NULL, str.c_str(), 0) + 1;
	wchar_t* dBuf = TRACE_NEW_ARRAY(wchar_t, dSize, dBuf);
	wmemset(dBuf, 0, dSize);
	mbstowcs(dBuf, str.c_str(), dSize);
	curName = setlocale(LC_ALL, oldname.c_str());
	std::wstring strText = dBuf;
	TRACE_DELETE_ARRAY(dBuf);
	return strText;
}

string StringUtility::UnicodeToANSI(const std::wstring& str)
{
	if (str == L"")
	{
		return EMPTY_STRING;
	}
	string oldname = setlocale(LC_ALL, NULL);
	string curName = setlocale(LC_ALL, LC_NAME_zh_CN_GBK);

	int dSize = wcstombs(NULL, str.c_str(), 0) + 1;
	char *dBuf = TRACE_NEW_ARRAY(char, dSize, dBuf);
	memset(dBuf, 0, dSize);
	wcstombs(dBuf, str.c_str(), dSize);
	curName = setlocale(LC_ALL, oldname.c_str());
	string strText = dBuf;
	TRACE_DELETE_ARRAY(dBuf);
	return strText;
}
string StringUtility::UnicodeToUTF8(const std::wstring& str)
{
	if (str == L"")
	{
		return EMPTY_STRING;
	}
	string oldname = setlocale(LC_ALL, NULL);
	string curName = setlocale(LC_ALL, LC_NAME_zh_CN_UTF8);

	int dSize = wcstombs(NULL, str.c_str(), 0) + 1;
	char *dBuf = TRACE_NEW_ARRAY(char, dSize, dBuf);
	memset(dBuf, 0, dSize);
	wcstombs(dBuf, str.c_str(), dSize);
	curName = setlocale(LC_ALL, oldname.c_str());
	string strText = dBuf;
	TRACE_DELETE_ARRAY(dBuf);
	return strText;
}

std::wstring StringUtility::UTF8ToUnicode(const string& str)
{
	if (str == EMPTY_STRING)
	{
		return L"";
	}

	string oldname = setlocale(LC_ALL, NULL);
	string curName = setlocale(LC_ALL, LC_NAME_zh_CN_UTF8);

	int dSize = mbstowcs(NULL, str.c_str(), 0) + 1;
	wchar_t* dBuf = TRACE_NEW_ARRAY(wchar_t, dSize, dBuf);
	wmemset(dBuf, 0, dSize);
	mbstowcs(dBuf, str.c_str(), dSize);
	curName = setlocale(LC_ALL, oldname.c_str());
	std::wstring strText = dBuf;
	TRACE_DELETE_ARRAY(dBuf);
	return strText;
}
#endif

string StringUtility::ANSIToUTF8(const string& str, bool addBOM)
{
	std::wstring unicodeStr = ANSIToUnicode(str);
	string utf8Str = UnicodeToUTF8(unicodeStr);
	if (addBOM)
	{
		char bom[4] = { (char)0xEF, (char)0xBB, (char)0xBF, 0 };
		utf8Str = string(bom) + utf8Str;
	}
	return utf8Str;
}

string StringUtility::UTF8ToANSI(const string& str, bool removeBOM)
{
	std::wstring unicodeStr;
	if (removeBOM && str.length() >= 3 && str[0] == 0xEF && str[0] == 0xBB && str[0] == 0xBF)
	{
		string newStr = str;
		newStr = newStr.erase(0, 3);
		unicodeStr = UTF8ToUnicode(newStr);
	}
	else
	{
		unicodeStr = UTF8ToUnicode(str);
	}
	string ansiStr = UnicodeToANSI(unicodeStr);
	return ansiStr;
}

void StringUtility::jsonStartArray(string& str, int preTableCount, bool returnLine)
{
	for (int i = 0; i < preTableCount; ++i)
	{
		str += "\t";
	}
	str += "[";
	if (returnLine)
	{
		str += "\r\n";
	}
}

void StringUtility::jsonEndArray(string& str, int preTableCount, bool returnLine)
{
	removeLastComma(str);
	for (int i = 0; i < preTableCount; ++i)
	{
		str += "\t";
	}
	str += "],";
	if (returnLine)
	{
		str += "\r\n";
	}
}

void StringUtility::jsonStartStruct(string& str, int preTableCount, bool returnLine)
{
	for (int i = 0; i < preTableCount; ++i)
	{
		str += "\t";
	}
	str += "{";
	if (returnLine)
	{
		str += "\r\n";
	}
}

void StringUtility::jsonEndStruct(string& str, int preTableCount, bool returnLine)
{
	removeLastComma(str);
	for (int i = 0; i < preTableCount; ++i)
	{
		str += "\t";
	}
	str += "},";
	if (returnLine)
	{
		str += "\r\n";
	}
}

void StringUtility::jsonAddPair(string& str, const string& name, const string& value, int preTableCount, bool returnLine)
{
	for (int i = 0; i < preTableCount; ++i)
	{
		str += "\t";
	}
	str += "\"" + name + "\": \"" + value + "\",";
	if (returnLine)
	{
		str += "\r\n";
	}
}

void StringUtility::strToLower(string& str)
{
	string::size_type size = str.length();
	for (string::size_type i = 0; i != size; ++i)
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
		{
			str[i] += 'a' - 'A';
		}
	}
}

void StringUtility::strToUpper(string& str)
{
	string::size_type size = str.length();
	for (string::size_type i = 0; i != size; ++i)
	{
		if (str[i] >= 'a' && str[i] <= 'z')
		{
			str[i] -= 'a' - 'A';
		}
	}
}

void StringUtility::rightToLeft(string& str)
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

bool StringUtility::findSubstr(string res, string dst, bool sensitive, int* pos, int startPose, bool firstOrLast)
{
	// 如果不区分大小写
	if (!sensitive)
	{
		// 全转换为小写
		strToLower(res);
		strToLower(dst);
	}
	int posFind = -1;
	int subLen = (int)dst.length();
	int searchLength = res.length() - subLen;
	int start = firstOrLast ? startPose : searchLength;
	int end = firstOrLast ? searchLength : startPose;
	int delta = firstOrLast ? 1 : -1;
	for (int i = start; i != end; i += delta)
	{
		int j = 0;
		for (j = 0; j != subLen; ++j)
		{
			if (res[i + j] != dst[j])
			{
				break;
			}
		}
		if (j == subLen)
		{
			posFind = i;
		}
	}
	if (pos != NULL)
	{
		*pos = posFind;
	}
	return posFind != -1;
}

txVector<string> StringUtility::findSubstr(txVector<string>& res, const string& dst, bool sensitive)
{
	txVector<string> retList;
	int listSize = res.size();
	FOR(res, int i = 0; i < listSize; ++i)
	{
		if (findSubstr(res[i], dst, sensitive))
		{
			retList.push_back(res[i]);
		}
	}
	END(res);
	return retList;
}

string StringUtility::checkString(const string& str, const string& valid)
{
	string newString = "";
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

string StringUtility::checkFloatString(const string& str, const string& valid)
{
	return checkIntString(str, "." + valid);
}

string StringUtility::checkIntString(const string& str, const string& valid)
{
	return checkString(str, "0123456789" + valid);
}

string StringUtility::charToHexString(unsigned char byte, bool upper)
{
	char byteHex[3] = { 0 };
	const char* charPool = upper ? "ABCDEF" : "abcdef";
	unsigned char highBit = byte >> 4;
	// 高字节的十六进制
	byteHex[0] = (highBit < (unsigned char)10) ? ('0' + highBit) : charPool[highBit - 10];
	// 低字节的十六进制
	unsigned char lowBit = byte & 0x0F;
	byteHex[1] = (lowBit < (unsigned char)10) ? ('0' + lowBit) : charPool[lowBit - 10];
	return byteHex;
}

string StringUtility::charArrayToHexString(unsigned char* data, int dataCount, bool addSpace, bool upper)
{
	int oneLength = addSpace ? 3 : 2;
	int showCount = dataCount * oneLength + 1;
	char* byteData = TRACE_NEW_ARRAY(char, showCount, byteData);
	memset(byteData, 0, showCount);
	for (int j = 0; j < dataCount; ++j)
	{
		string byteStr = charToHexString(data[j]);
		byteData[j * oneLength + 0] = byteStr[0];
		byteData[j * oneLength + 1] = byteStr[1];
		if (oneLength >= 3)
		{
			byteData[j * oneLength + 2] = ' ';
		}
	}
	string str(byteData);
	TRACE_DELETE_ARRAY(byteData);
	return str;
}

int StringUtility::getCharCount(const string& str, char key)
{
	int count = 0;
	int length = str.length();
	for (int i = 0; i < length; ++i)
	{
		if (str[i] == key)
		{
			++count;
		}
	}
	return count;
}

int StringUtility::numberStringAddInt(const string& str)
{
	int length = str.length();
	int ret = 0;
	for (int i = 0; i < length; ++i)
	{
		ret += str[i] - '0';
	}
	return ret;
}

void StringUtility::appendValueString(string& queryStr, const string& str, bool toUTF8)
{
	if (toUTF8)
	{
		queryStr += "\"" + ANSIToUTF8(str) + "\",";
	}
	else
	{
		queryStr += "\"" + str + "\",";
	}
}
void StringUtility::appendValueInt(string& queryStr, int value)
{
	queryStr += intToString(value) + ",";
}
void StringUtility::appendValueIntArray(string& queryStr, const txVector<int>& intArray)
{
	appendValueString(queryStr, intArrayToString(intArray));
}
void StringUtility::appendValueVector2i(string& queryStr, const Vector2i& value)
{
	appendValueString(queryStr, intToString(value.x) + "," + intToString(value.y));
}
void StringUtility::appendValueFloat(string& queryStr, float value)
{
	queryStr += floatToString(value) + ",";
}
void StringUtility::appendValueFloatArray(string& queryStr, const txVector<float>& floatArray)
{
	appendValueString(queryStr, floatArrayToString(floatArray));
}
void StringUtility::appendConditionString(string& condition, const string& col, const string& str, const string& operate)
{
	condition += col + " = " + "\"" + ANSIToUTF8(str) + "\"" + operate;
}
void StringUtility::appendConditionInt(string& condition, const string& col, int value, const string& operate)
{
	condition += col + " = " + intToString(value) + operate;
}
void StringUtility::appendUpdateString(string& updateInfo, const string& col, const string& str, bool toUTF8)
{
	if (toUTF8)
	{
		updateInfo += col + " = " + "\"" + ANSIToUTF8(str) + "\",";
	}
	else
	{
		updateInfo += col + " = " + "\"" + str + "\",";
	}
}
void StringUtility::appendUpdateInt(string& updateInfo, const string& col, int value)
{
	updateInfo += col + " = " + intToString(value) + ",";
}
void StringUtility::appendUpdateIntArray(string& updateInfo, const string& col, const txVector<int>& intArray)
{
	appendUpdateString(updateInfo, col, intArrayToString(intArray));
}
void StringUtility::appendUpdateVector2i(string& updateInfo, const string& col, const Vector2i& value)
{
	appendUpdateString(updateInfo, col, intToString(value.x) + "," + intToString(value.y));
}
void StringUtility::appendUpdateFloat(string& updateInfo, const string& col, float value)
{
	updateInfo += col + " = " + floatToString(value) + ",";
}
void StringUtility::appendUpdateFloatArray(string& updateInfo, const string& col, const txVector<float>& floatArray)
{
	appendUpdateString(updateInfo, col, floatArrayToString(floatArray));
}