#ifndef _TX_UTILITY_H_
#define _TX_UTILITY_H_

#include "DataDefine.h"

class txUtility
{
public:
	// 返回media的路径,不带/
	static std::string getMediaPath();
	static void rightToLeft(std::string& str);
	static std::string getAvailableResourcePath(std::string name);
	static void split(std::string str, const char* deli, std::vector<std::string>* vec);
	static void findFiles(const char * pathName, std::vector<std::string>& files, const std::vector<std::string>& patterns);
	static bool isFileExist(std::string fullPath);
	static bool createFolder(std::string path);
	static bool writeFile(std::string filePath, int length, const char* buffer);
	static bool loadFile(const std::string& fullFileName, char*& buffer, int& bufferSize, bool addZero);
	static std::string intToString(int i, int limitLen = 0); // limitLen是字符串的最小长度,如果整数的位数不足最小长度,则会在前面加0
	static int stringToInt(const std::string& str);
	static float pow_f(float f, int p);
	static std::string floatToString(float f, int precision);	//precision -> 精度,保留的小数的位数
	static float stringToFloat(std::string str);
	static std::string boolToString(bool value);
	static bool stringToBool(const std::string& str);
	static std::string getFileNameNoSuffix(std::string str);
	static std::string getFileName(std::string str);
	static bool endWith(const std::string& oriString, const std::string& pattern);// 判断oriString是否以pattern结尾
	static bool startWith(const std::string& oriString, const std::string& pattern);// 判断oriString是否以pattern开头
	static std::string strReplace(const std::string& str, int begin, int end, const std::string& reStr);
	static float calculateFloat(std::string str);	// 以浮点数的计算法则计算一个表达式,只支持加减乘除和括号
	static int calculateInt(std::string str);		// 以整数的计算法则计算一个表达式,支持取余,加减乘除和括号
	static int strFind(const std::string& res, const std::string& sub, int startPos, bool fromStart);
	static bool findSubstr(std::string res, std::string dst, bool sensitive, int* pos = NULL, int startPose = 0, bool firstOrLast = true);
	static void strToLower(std::string& str);
	static void strToUpper(std::string& str);
};

#endif