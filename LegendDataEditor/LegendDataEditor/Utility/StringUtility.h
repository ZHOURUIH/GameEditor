#ifndef _STRING_UTILITY_H_
#define _STRING_UTILITY_H_

#include "ServerDefine.h"

class StringUtility
{
public:
	static std::string removeSuffix(const std::string& str);
	// 去除最后一个指定的字符
	static void removeLast(std::string& stream, const std::string& key);
	// 去掉最后一个逗号
	static void removeLastComma(std::string& stream);
	static std::string getFolderName(std::string str);
	static std::string getFileName(std::string str);
	static std::string getFileNameNoSuffix(std::string str, bool removePath = true);
	static std::string getFilePath(std::string dir);
	static std::string getFileSuffix(const std::string& fileName, bool includeDot = false);
	// 获得字符串最后不是数字的下标
	static int getLastNotNumberPos(const std::string& str);
	static std::string getNotNumberSubString(const std::string& str);
	// 获得字符串结尾的数字
	static int getLastNumber(const std::string& str);
	static void split(std::string str, const std::string& deli, txVector<std::string>& vec, bool removeEmpty = true);
	static std::string boolToString(bool value) { return value ? "True" : "False"; }
	static bool stringToBool(const std::string& str) { return str == "True" || str == "true"; }
	// 将str中的[begin,end)替换为reStr
	static std::string strReplace(const std::string& str, int begin, int end, const std::string& reStr);
	// limitLen是字符串的最小长度,如果整数的位数不足最小长度,则会在前面加0
	static std::string intToString(int i, int limitLen = 0); 
	static std::string intArrayToString(const txVector<int>& intArray, int limitLen = 0, const std::string& key = ",");
	static int stringToInt(const std::string& str){return atoi(str.c_str());}
	static void stringToIntArray(const std::string& str, txVector<int>& intArray, const std::string& key = ",");
	//precision为精度,保留的小数的位数,removeZero为是否去除末尾无用的0
	static std::string floatToString(float f, int precision = 4, bool removeZero = true);
	static float stringToFloat(const std::string& str){return (float)atof(str.c_str());}
	static void stringToFloatArray(const std::string& str, float*& floatArray, const std::string& key = ",");
	static void stringToFloatArray(const std::string& str, txVector<float>& floatArray, const std::string& key = ",");
	static std::string floatArrayToString(const txVector<float>& floatArray, const std::string& key = ",");
	// 判断oriString是否以pattern结尾,sensitive为是否大小写敏感
	static bool endWith(const std::string& oriString, const std::string& pattern, bool sensitive = true);
	// 判断oriString是否以pattern开头,sensitive为是否大小写敏感
	static bool startWith(const std::string& oriString, const std::string& pattern, bool sensitive = true);
	static std::wstring ANSIToUnicode(const std::string& str);
	static std::string UnicodeToANSI(const std::wstring& str);
	static std::string UnicodeToUTF8(const std::wstring& str);
	static std::wstring UTF8ToUnicode(const std::string& str);
	static std::string ANSIToUTF8(const std::string& str, bool addBOM = false);
	static std::string UTF8ToANSI(const std::string& str, bool removeBOM = false);
	// json
	static void jsonStartArray(std::string& str, int preTableCount = 0, bool returnLine = false);
	static void jsonEndArray(std::string& str, int preTableCount = 0, bool returnLine = false);
	static void jsonStartStruct(std::string& str, int preTableCount = 0, bool returnLine = false);
	static void jsonEndStruct(std::string& str, int preTableCount = 0, bool returnLine = false);
	static void jsonAddPair(std::string& str, const std::string& name, const std::string& value, int preTableCount = 0, bool returnLine = false);
	static void strToLower(std::string& str);
	static void strToUpper(std::string& str);
	static void rightToLeft(std::string& str);
	static bool findSubstr(std::string res, std::string dst, bool sensitive, int* pos = NULL, int startPose = 0, bool firstOrLast = true);
	static txVector<std::string> findSubstr(txVector<std::string>& res, const std::string& dst, bool sensitive = true);
	template<typename T>
	static txVector<std::string> findSubstr(txMap<std::string, T>& res, const std::string& dst, bool sensitive = true)
	{
		txVector<std::string> retList;
		// 循环遍历,如果匹配到放入列表
		auto itr = res.begin();
		auto itrEnd = res.end();
		FOR(res, ; itr != itrEnd; ++itr)
		{
			const std::string& name = itr->first;
			if (findSubstr(name, dst, sensitive))
			{
				retList.push_back(name);
			}
		}
		END(res);
		return retList;
	}
	static std::string checkString(const std::string& str, const std::string& valid);
	static std::string checkFloatString(const std::string& str, const std::string& valid = "");
	static std::string checkIntString(const std::string& str, const std::string& valid = "");
	static std::string charToHexString(unsigned char byte, bool upper = true);
	static int getCharCount(const std::string& str, char key);
	static std::string charArrayToHexString(unsigned char* data, int dataCount, bool addSpace = true, bool upper = true);
	static int numberStringAddInt(const std::string& str);
};

#endif