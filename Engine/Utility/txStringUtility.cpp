#include "txStringUtility.h"
#include "Utility.h"

std::string txStringUtility::removeSuffix(const std::string& str)
{
	int dotPos = str.find_last_of('.');
	if (dotPos != -1)
	{
		return str.substr(0, dotPos);
	}
	return str;
}

void txStringUtility::removeLastComma(std::string& stream)
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

std::string txStringUtility::getFileName(std::string str)
{
	rightToLeft(str);
	int dotPos = str.find_last_of('/');
	if (dotPos != -1)
	{
		return str.substr(dotPos + 1, str.length() - 1);
	}
	return str;
}

std::string txStringUtility::getFileNameNoSuffix(std::string str)
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

std::string txStringUtility::getFilePath(std::string dir)
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

std::string txStringUtility::getFileSuffix(const std::string& fileName)
{
	int dotPos = fileName.find_last_of('.');
	if (dotPos != -1)
	{
		return fileName.substr(dotPos + 1, fileName.length() - dotPos - 1);
	}
	return fileName;
}

int txStringUtility::getLastNotNumberPos(const std::string& str)
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

int txStringUtility::getLastNumber(const std::string& str)
{
	int lastPos = getLastNotNumberPos(str);
	if (lastPos == -1)
	{
		return -1;
	}
	std::string numStr = str.substr(lastPos + 1, str.length() - lastPos - 1);
	if (numStr == EMPTY_STRING)
	{
		return 0;
	}
	return stringToInt(numStr);
}

void txStringUtility::split(std::string str, const std::string& deli, txVector<std::string>& vec)
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
			str = str.substr(devidePos + deli.length(), str.length() - devidePos - deli.length());
		}
	}
}

VECTOR2 txStringUtility::stringToVector2(const std::string& value)
{
	txVector<std::string> vec;
	split(value, ",", vec);
	if (vec.size() < 2)
	{
		return VECTOR2();
	}
	VECTOR2 v2;
	v2.x = stringToFloat(vec[0].c_str());
	v2.y = stringToFloat(vec[1].c_str());
	return v2;
}

VECTOR3 txStringUtility::stringToVector3(const std::string& value)
{
	txVector< std::string > vec;
	split(value, ",", vec);
	if (vec.size() < 3)
	{
		return txMath::VEC3_ZERO;
	}
	VECTOR3 v3;
	v3.x = stringToFloat(vec[0].c_str());
	v3.y = stringToFloat(vec[1].c_str());
	v3.z = stringToFloat(vec[2].c_str());
	return v3;
}

VECTOR4 txStringUtility::stringToVector4(const std::string& value)
{
	txVector< std::string > vec;
	split(value, ",", vec);
	if (vec.size() < 4)
	{
		return VECTOR4();
	}

	VECTOR4 v4;
	v4.x = stringToFloat(vec[0].c_str());
	v4.y = stringToFloat(vec[1].c_str());
	v4.z = stringToFloat(vec[2].c_str());
	return v4;
}

std::string txStringUtility::vector2ToString(const VECTOR2& vec)
{
	char str[64];
	SPRINTF(str, 64, "%.4f,%.4f", vec.x, vec.y);
	return std::string(str);
}

std::string txStringUtility::vector3ToString(const VECTOR3& vec)
{
	char str[64];
	SPRINTF(str, 64, "%.4f,%.4f,%.4f", vec.x, vec.y, vec.z);
	return std::string(str);
}

std::string txStringUtility::vector4ToString(const VECTOR4& vec)
{
	char str[64];
	SPRINTF(str, 64, "%.4f,%.4f,%.4f,%.4f", vec.x, vec.y, vec.z, vec.w);
	return std::string(str);
}

std::string txStringUtility::strReplace(const std::string& str, const int& begin, const int& end, const std::string& reStr)
{
	std::string sub1 = str.substr(0, begin);
	std::string sub2 = str.substr(end, str.length() - end);
	return sub1 + reStr + sub2;
}

std::string txStringUtility::intToString(const int& i, const int& limitLen)
{
	char str[256];
	SPRINTF(str, 256, "%d", i);
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

std::string txStringUtility::floatToString(float f, const int& precision, const bool& removeZero)
{
	std::string retString;
	for (int temp = 0; temp < 1; ++temp)
	{
		if (!txMath::isFloatZero(f))
		{
			f = f * txMath::powerFloat(10.0f, precision) + f / std::abs(f) * 0.5f;
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
				retString = std::string(intStr) + "." + std::string(decimalStr);
			}
			else
			{
				char intStr[256];
				memset(intStr, 0, 256 * sizeof(char));
				char decimalStr[256];
				memset(decimalStr, 0, 256 * sizeof(char));
				memcpy(intStr, str, strlen(str) - precision);
				memcpy(decimalStr, str + strlen(str) - precision, precision);
				retString = std::string(intStr) + "." + std::string(decimalStr);
			}
		}
	}
	// 移除末尾无用的0
	if (removeZero)
	{
		int dotPos = retString.find_last_of('.');
		if (dotPos != -1)
		{
			std::string floatPart = retString.substr(dotPos + 1, retString.length() - dotPos - 1);
			// 找到最后一个不是0的位置,然后将后面的所有0都去掉
			int notZeroPos = floatPart.find_last_not_of('0');
			// 如果小数部分全是0,则将小数点也一起去掉
			if (notZeroPos == -1)
			{
				retString = retString.substr(0, dotPos);
			}
			else if (notZeroPos != floatPart.length() - 1)
			{
				floatPart = floatPart.substr(0, notZeroPos + 1);
				retString = retString.substr(0, dotPos + 1) + floatPart;
			}
		}
	}
	return retString;
}

bool txStringUtility::endWith(const std::string& oriString, const std::string& pattern, const bool& sensitive)
{
	if (oriString.length() < pattern.length())
	{
		return false;
	}
	std::string endString = oriString.substr(oriString.length() - pattern.length(), pattern.length());
	if (sensitive)
	{
		return endString == pattern;
	}
	else
	{
		strToLower(endString);
		std::string temp = pattern;
		strToLower(temp);
		return endString == temp;
	}
}

bool txStringUtility::startWith(const std::string& oriString, const std::string& pattern, const bool& sensitive)
{
	if (oriString.length() < pattern.length())
	{
		return false;
	}
	std::string startString = oriString.substr(0, pattern.length());
	if (sensitive)
	{
		return startString == pattern;
	}
	else
	{
		strToLower(startString);
		std::string temp = pattern;
		strToLower(temp);
		return startString == temp;
	}
}

#if RUN_PLATFORM == PLATFORM_WINDOWS
std::wstring txStringUtility::ANSIToUnicode(const std::string& str)
{
	int unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	wchar_t* pUnicode = TRACE_NEW_ARRAY(wchar_t, unicodeLen + 1, pUnicode);
	memset(pUnicode, 0, (unicodeLen + 1)*sizeof(wchar_t));
	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, (LPWSTR)pUnicode, unicodeLen);
	std::wstring rt = (wchar_t*)pUnicode;
	TRACE_DELETE_ARRAY(pUnicode);
	return rt;
}

std::string txStringUtility::UnicodeToANSI(const std::wstring& str)
{
	int iTextLen = WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
	char* pElementText = TRACE_NEW_ARRAY(char, iTextLen + 1, pElementText);
	memset((void*)pElementText, 0, sizeof(char)* (iTextLen + 1));
	::WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, pElementText, iTextLen, NULL, NULL);
	std::string strText = pElementText;
	TRACE_DELETE_ARRAY(pElementText);
	return strText;
}
std::string txStringUtility::UnicodeToUTF8(const std::wstring& str)
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

std::wstring txStringUtility::UTF8ToUnicode(const std::string& str)
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
std::wstring txStringUtility::ANSIToUnicode(const std::string& str)
{
	if (str == EMPTY_STRING)
	{
		return L"";
	}

	std::string oldname = setlocale(LC_ALL, NULL);
	std::string curName = setlocale(LC_ALL, LC_NAME_zh_CN_GBK);

	int dSize = mbstowcs(NULL, str.c_str(), 0) + 1;

	wchar_t* dBuf = TRACE_NEW_ARRAY(wchar_t, dSize, dBuf);
	wmemset(dBuf, 0, dSize);
	int nRet = mbstowcs(dBuf, str.c_str(), dSize);

	curName = setlocale(LC_ALL, oldname.c_str());
	std::wstring strText = dBuf;
	TRACE_DELETE_ARRAY(dBuf);
	return strText;
}

std::string txStringUtility::UnicodeToANSI(const std::wstring& str)
{
	if (str == L"")
	{
		return EMPTY_STRING;
	}
	std::string oldname = setlocale(LC_ALL, NULL);
	std::string curName = setlocale(LC_ALL, LC_NAME_zh_CN_GBK);

	int dSize = wcstombs(NULL, str.c_str(), 0) + 1;

	char *dBuf = TRACE_NEW_ARRAY(char, dSize, dBuf);
	memset(dBuf, 0, dSize);

	int nRet = wcstombs(dBuf, str.c_str(), dSize);

	curName = setlocale(LC_ALL, oldname.c_str());
	std::string strText = dBuf;
	TRACE_DELETE_ARRAY(dBuf);
	return strText;
}
std::string txStringUtility::UnicodeToUTF8(const std::wstring& str)
{
	if (str == L"")
	{
		return EMPTY_STRING;
	}
	std::string oldname = setlocale(LC_ALL, NULL);
	std::string curName = setlocale(LC_ALL, LC_NAME_zh_CN_UTF8);

	int dSize = wcstombs(NULL, str.c_str(), 0) + 1;
	char *dBuf = TRACE_NEW_ARRAY(char, dSize, dBuf);
	memset(dBuf, 0, dSize);
	int nRet = wcstombs(dBuf, str.c_str(), dSize);

	curName = setlocale(LC_ALL, oldname.c_str());
	std::string strText = dBuf;
	TRACE_DELETE_ARRAY(dBuf);
	return strText;
}

std::wstring txStringUtility::UTF8ToUnicode(const std::string& str)
{
	if (str == EMPTY_STRING)
	{
		return L"";
	}

	std::string oldname = setlocale(LC_ALL, NULL);
	std::string curName = setlocale(LC_ALL, LC_NAME_zh_CN_UTF8);

	int dSize = mbstowcs(NULL, str.c_str(), 0) + 1;
	wchar_t* dBuf = TRACE_NEW_ARRAY(wchar_t, dSize, dBuf);
	wmemset(dBuf, 0, dSize);
	int nRet = mbstowcs(dBuf, str.c_str(), dSize);

	curName = (LC_ALL, oldname.c_str());
	std::wstring strText = dBuf;
	TRACE_DELETE_ARRAY(dBuf);
	return strText;
}
#endif

std::string txStringUtility::ANSIToUTF8(const std::string& str, const bool& addBOM)
{
	std::wstring unicodeStr = ANSIToUnicode(str);
	std::string utf8Str = UnicodeToUTF8(unicodeStr);
	if (addBOM)
	{
		char bom[4] = { (char)0xEF, (char)0xBB, (char)0xBF, 0 };
		utf8Str = std::string(bom) + utf8Str;
	}
	return utf8Str;
}

std::string txStringUtility::UTF8ToANSI(const std::string& str, const bool& removeBOM)
{
	std::wstring unicodeStr;
	if (removeBOM && str.length() >= 3 && str[0] == 0xEF && str[0] == 0xBB && str[0] == 0xBF)
	{
		std::string newStr = str;
		newStr = newStr.erase(0, 3);
		unicodeStr = UTF8ToUnicode(newStr);
	}
	else
	{
		unicodeStr = UTF8ToUnicode(str);
	}
	std::string ansiStr = UnicodeToANSI(unicodeStr);
	return ansiStr;
}

void txStringUtility::jsonStartArray(std::string& str, const int& preTableCount, const bool& returnLine)
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

void txStringUtility::jsonEndArray(std::string& str, const int& preTableCount, const bool& returnLine)
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

void txStringUtility::jsonStartStruct(std::string& str, const int& preTableCount, bool returnLine)
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

void txStringUtility::jsonEndStruct(std::string& str, const int& preTableCount, const bool& returnLine)
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

void txStringUtility::jsonAddPair(std::string& str, const std::string& name, const std::string& value, const int& preTableCount, const bool& returnLine)
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

void txStringUtility::strToLower(std::string& str)
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

void txStringUtility::strToUpper(std::string& str)
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

txDim txStringUtility::stringToDim(const std::string& value)
{
	txVector<std::string> vec;
	split(value, ",", vec);
	if (vec.size() < 2)
	{
		return txDim::ZERO;
	}
	txDim p;
	p.mRelative = stringToFloat(vec[0].c_str());
	p.mAbsolute = stringToInt(vec[1].c_str());
	return p;
}

std::string txStringUtility::dimToString(const txDim& d)
{
	char str[32];
	SPRINTF(str, 32, "%.4f,%d", d.mRelative, d.mAbsolute);
	return std::string(str);
}

txPoint txStringUtility::stringToPoint(const std::string& value)
{
	txVector<std::string> vec;
	split(value, ",", vec);
	if (vec.size() < 4)
	{
		return txPoint();
	}
	txPoint p;
	p.x.mRelative = stringToFloat(vec[0].c_str());
	p.x.mAbsolute = stringToInt(vec[1].c_str());
	p.y.mRelative = stringToFloat(vec[2].c_str());
	p.y.mAbsolute = stringToInt(vec[3].c_str());
	return p;
}

std::string txStringUtility::pointToString(const txPoint& p)
{
	char str[64];
	SPRINTF(str, 64, "%.4f,%d,%.4f,%d", p.x.mRelative, p.x.mAbsolute, p.y.mRelative, p.y.mAbsolute);
	return std::string(str);
}

void txStringUtility::rightToLeft(std::string& str)
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

bool txStringUtility::findSubstr(std::string res, std::string dst, const bool& sensitive, int* pos, const int& startPose, const bool& firstOrLast)
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

txVector<std::string> txStringUtility::findSubstr(txVector<std::string>& res, const std::string& dst, const bool& sensitive)
{
	txVector<std::string> retList;
	int listSize = res.size();
	FOR_STL(res, int i = 0; i < listSize; ++i)
	{
		if (findSubstr(res[i], dst, sensitive))
		{
			retList.push_back(res[i]);
		}
	}
	END_FOR_STL(res);
	return retList;
}

std::string txStringUtility::checkString(const std::string& str, const std::string& valid)
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

std::string txStringUtility::checkFloatString(const std::string& str, const std::string& valid)
{
	return checkIntString(str, "." + valid);
}

std::string txStringUtility::checkIntString(const std::string& str, const std::string& valid)
{
	return checkString(str, "0123456789" + valid);
}

std::string txStringUtility::charToHexString(const unsigned char& byte, const bool& upper)
{
	char byteHex[3] = { 0 };
	char* charPool = upper ? "ABCDEF" : "abcdef";
	unsigned char highBit = byte >> 4;
	// 高字节的十六进制
	byteHex[0] = (highBit < (unsigned char)10) ? ('0' + highBit) : charPool[highBit - 10];
	// 低字节的十六进制
	unsigned char lowBit = byte & 0x0F;
	byteHex[1] = (lowBit < (unsigned char)10) ? ('0' + lowBit) : charPool[lowBit - 10];
	return byteHex;
}

std::string txStringUtility::charArrayToHexString(unsigned char* data, const int& dataCount, const bool& addSpace, const bool& upper)
{
	int oneLength = addSpace ? 3 : 2;
	int showCount = dataCount * oneLength + 1;
	char* byteData = TRACE_NEW_ARRAY(char, showCount, byteData);
	memset(byteData, 0, showCount);
	for (int j = 0; j < dataCount; ++j)
	{
		std::string byteStr = charToHexString(data[j]);
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