#ifndef _STRING_UTILITY_H_
#define _STRING_UTILITY_H_

#include "ServerDefine.h"

class StringUtility
{
public:
	static string removeSuffix(const string& str);
	// 去除最后一个指定的字符
	static void removeLast(string& stream, const string& key);
	// 去掉最后一个逗号
	static void removeLastComma(string& stream);
	static string getFolderName(string str);
	static string getFileName(string str);
	static string getFileNameNoSuffix(string str, bool removePath = true);
	static string getFilePath(string dir);
	static string getFileSuffix(const string& fileName, bool includeDot = false);
	// 获得字符串最后不是数字的下标
	static int getLastNotNumberPos(const string& str);
	static string getNotNumberSubString(const string& str);
	// 获得字符串结尾的数字
	static int getLastNumber(const string& str);
	static void split(string str, const string& deli, txVector<string>& vec, bool removeEmpty = true);
	static string boolToString(bool value) { return value ? "True" : "False"; }
	static bool stringToBool(const string& str) { return str == "True" || str == "true"; }
	// 将str中的[begin,end)替换为reStr
	static string strReplace(const string& str, int begin, int end, const string& reStr);
	static void StringUtility::replaceAll(string& str, const char* string_to_replace, const char* new_string);
	// limitLen是字符串的最小长度,如果整数的位数不足最小长度,则会在前面加0
	static string intToString(int i, int limitLen = 0); 
	static string intArrayToString(const txVector<int>& intArray, int limitLen = 0, const string& key = ",");
	static int stringToInt(const string& str){return atoi(str.c_str());}
	static void stringToIntArray(const string& str, txVector<int>& intArray, const string& key = ",");
	//precision为精度,保留的小数的位数,removeZero为是否去除末尾无用的0
	static string floatToString(float f, int precision = 4, bool removeZero = true);
	static float stringToFloat(const string& str){return (float)atof(str.c_str());}
	static void stringToFloatArray(const string& str, float*& floatArray, const string& key = ",");
	static void stringToFloatArray(const string& str, txVector<float>& floatArray, const string& key = ",");
	static string floatArrayToString(const txVector<float>& floatArray, const string& key = ",");
	// 判断oriString是否以pattern结尾,sensitive为是否大小写敏感
	static bool endWith(const string& oriString, const string& pattern, bool sensitive = true);
	// 判断oriString是否以pattern开头,sensitive为是否大小写敏感
	static bool startWith(const string& oriString, const string& pattern, bool sensitive = true);
	static std::wstring ANSIToUnicode(const string& str);
	static string UnicodeToANSI(const std::wstring& str);
	static string UnicodeToUTF8(const std::wstring& str);
	static std::wstring UTF8ToUnicode(const string& str);
	static string ANSIToUTF8(const string& str, bool addBOM = false);
	static string UTF8ToANSI(const string& str, bool removeBOM = false);
	// json
	static void jsonStartArray(string& str, int preTableCount = 0, bool returnLine = false);
	static void jsonEndArray(string& str, int preTableCount = 0, bool returnLine = false);
	static void jsonStartStruct(string& str, int preTableCount = 0, bool returnLine = false);
	static void jsonEndStruct(string& str, int preTableCount = 0, bool returnLine = false);
	static void jsonAddPair(string& str, const string& name, const string& value, int preTableCount = 0, bool returnLine = false);
	static void strToLower(string& str);
	static void strToUpper(string& str);
	static void rightToLeft(string& str);
	static bool findSubstr(string res, string dst, bool sensitive, int* pos = NULL, int startPose = 0, bool firstOrLast = true);
	static txVector<string> findSubstr(txVector<string>& res, const string& dst, bool sensitive = true);
	template<typename T>
	static txVector<string> findSubstr(txMap<string, T>& res, const string& dst, bool sensitive = true)
	{
		txVector<string> retList;
		// 循环遍历,如果匹配到放入列表
		auto itr = res.begin();
		auto itrEnd = res.end();
		FOR(res, ; itr != itrEnd; ++itr)
		{
			const string& name = itr->first;
			if (findSubstr(name, dst, sensitive))
			{
				retList.push_back(name);
			}
		}
		END(res);
		return retList;
	}
	static string checkString(const string& str, const string& valid);
	static string checkFloatString(const string& str, const string& valid = "");
	static string checkIntString(const string& str, const string& valid = "");
	static string charToHexString(unsigned char byte, bool upper = true);
	static int getCharCount(const string& str, char key);
	static string charArrayToHexString(unsigned char* data, int dataCount, bool addSpace = true, bool upper = true);
	static int numberStringAddInt(const string& str);
};

#endif