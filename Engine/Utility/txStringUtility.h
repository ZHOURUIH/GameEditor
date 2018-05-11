#ifndef _TX_STRING_UTILITY_H_
#define _TX_STRING_UTILITY_H_

#include "txEngineDefine.h"
#include "txPoint.h"
#include "txDim.h"

class txStringUtility
{
public:
	static std::string removeSuffix(const std::string& str);
	// 去掉最后一个逗号
	static void removeLastComma(std::string& stream);
	static std::string getFileName(std::string str);
	static std::string getFileNameNoSuffix(std::string str);
	static std::string getFilePath(std::string dir);
	static std::string getFileSuffix(const std::string& fileName);
	// 获得字符串最后不是数字的下标
	static int getLastNotNumberPos(const std::string& str);
	// 获得字符串结尾的数字
	static int getLastNumber(const std::string& str);
	static void split(std::string str, const std::string& deli, txVector<std::string>& vec);
	static std::string boolToString(const bool& value) { return value ? "True" : "False"; }
	static bool stringToBool(const std::string& str) { return str == "True" || str == "true"; }
	static VECTOR2 stringToVector2(const std::string& value);
	static VECTOR3 stringToVector3(const std::string& value);
	static VECTOR4 stringToVector4(const std::string& value);
	static std::string vector2ToString(const VECTOR2& vec);
	static std::string vector3ToString(const VECTOR3& vec);
	static std::string vector4ToString(const VECTOR4& vec);
	// 将str中的[begin,end)替换为reStr
	static std::string strReplace(const std::string& str, const int& begin, const int& end, const std::string& reStr);
	// limitLen是字符串的最小长度,如果整数的位数不足最小长度,则会在前面加0
	static std::string intToString(const int& i, const int& limitLen = 0); 
	static int stringToInt(const std::string& str){return atoi(str.c_str());}
	//precision为精度,保留的小数的位数,removeZero为是否去除末尾无用的0
	static std::string floatToString(float f, const int& precision = 4, const bool& removeZero = true);
	static float stringToFloat(const std::string& str){return (float)atof(str.c_str());}
	// 判断oriString是否以pattern结尾,sensitive为是否大小写敏感
	static bool endWith(const std::string& oriString, const std::string& pattern, const bool& sensitive = true);
	// 判断oriString是否以pattern开头,sensitive为是否大小写敏感
	static bool startWith(const std::string& oriString, const std::string& pattern, const bool& sensitive = true);
#if RUN_PLATFORM == PLATFORM_WINDOWS
	static std::wstring ANSIToUnicode(const std::string& str);
	static std::string UnicodeToANSI(const std::wstring& str);
	static std::string UnicodeToUTF8(const std::wstring& str);
	static std::wstring UTF8ToUnicode(const std::string& str);
#elif RUN_PLATFORM == PLATFORM_ANDROID
	static std::wstring ANSIToUnicode(const std::string& str);
	static std::string UnicodeToANSI(const std::wstring& str);
	static std::string UnicodeToUTF8(const std::wstring& str);
	static std::wstring UTF8ToUnicode(const std::string& str);
#endif
	static std::string ANSIToUTF8(const std::string& str, const bool& addBOM = false);
	static std::string UTF8ToANSI(const std::string& str, const bool& removeBOM = false);
	// json
	static void jsonStartArray(std::string& str, const int& preTableCount = 0, const bool& returnLine = false);
	static void jsonEndArray(std::string& str, const int& preTableCount = 0, const bool& returnLine = false);
	static void jsonStartStruct(std::string& str, const int& preTableCount = 0, bool returnLine = false);
	static void jsonEndStruct(std::string& str, const int& preTableCount = 0, const bool& returnLine = false);
	static void jsonAddPair(std::string& str, const std::string& name, const std::string& value, const int& preTableCount = 0, const bool& returnLine = false);
	static void strToLower(std::string& str);
	static void strToUpper(std::string& str);
	static txDim stringToDim(const std::string& value);
	static std::string dimToString(const txDim& d);
	static txPoint stringToPoint(const std::string& value);
	static std::string pointToString(const txPoint& p);
	static void rightToLeft(std::string& str);
	static bool findSubstr(std::string res, std::string dst, const bool& sensitive, int* pos = NULL, const int& startPose = 0, const bool& firstOrLast = true);
	static txVector<std::string> findSubstr(txVector<std::string>& res, const std::string& dst, const bool& sensitive = true);
	template<typename T>
	static txVector<std::string> findSubstr(txMap<std::string, T>& res, const std::string& dst, const bool& sensitive = true)
	{
		txVector<std::string> retList;
		// 循环遍历,如果匹配到放入列表
		auto itr = res.begin();
		auto itrEnd = res.end();
		FOR_STL(res, ; itr != itrEnd; ++itr)
		{
			const std::string& name = itr->first;
			if (findSubstr(name, dst, sensitive))
			{
				retList.push_back(name);
			}
		}
		END_FOR_STL(res);
		return retList;
	}
	static std::string checkString(const std::string& str, const std::string& valid);
	static std::string checkFloatString(const std::string& str, const std::string& valid = "");
	static std::string checkIntString(const std::string& str, const std::string& valid = "");
	static std::string charToHexString(const unsigned char& byte, const bool& upper = true);
	std::string charArrayToHexString(unsigned char* data, const int& dataCount, const bool& addSpace = true, const bool& upper = true);
};

#endif