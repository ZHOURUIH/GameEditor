#ifndef _TX_UTILITY_H_
#define _TX_UTILITY_H_

#include <vector>
#include <string>
#include <map>
#include <stdlib.h>

#include "txMemeryTrace.h"
#include "txLog.h"

//角度转弧度
#define AngleToRadian(angle) angle * 3.141592654 / 180.0

//弧度转角度
#define RadianToAngle(radian) radian * 180.0 / 3.141592654

#define TX_MAX(x, y) ((x) > (y) ? (x) : (y))
#define TX_MIN(x, y) ((x) < (y) ? (x) : (y))

class txUtility
{
public:
	static void logError(std::string info);
	static void findFiles(const char * pathName, std::vector<std::string>& files, std::vector<std::string> patterns, bool recursive = true);
	static void deleteFolder(std::string path);

	// 返回media的路径,不带/
	static std::string getMediaPath()
	{
		return "../media";
	}

	// 通过一个media下的相对路径,或者绝对路径,转化为一个可直接打开的路径
	static std::string getAvailableResourcePath(std::string name)
	{
		std::string mediaPath = txUtility::getMediaPath();
		// 如果文件名已经是不带media路径并且不是绝对路径
		if (mediaPath != "" && (name.length() <= mediaPath.length() || name.substr(0, mediaPath.length()) != mediaPath) && (name.length() > 1 && name[1] != ':'))
		{
			name = mediaPath + "/" + name;
		}
		return name;
	}

	static bool isFileExist(std::string fullPath);

	static std::string removeSuffix(std::string str)
	{
		int dotPos = str.find_last_of('.');
		if (dotPos != -1)
		{
			return str.substr(0, dotPos);
		}
		return str;
	}

	// 去掉最后一个逗号
	static void removeLastComma(std::string& stream)
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

	static std::string getFileName(std::string str)
	{
		rightToLeft(str);
		int dotPos = str.find_last_of('/');
		if (dotPos != -1)
		{
			return str.substr(dotPos + 1, str.length() - 1);
		}
		return str;
	}

	static std::string getFileNameNoSuffix(std::string str)
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
	static std::string getParentDir(std::string dir)
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

	static std::string getFileSuffix(std::string fileName)
	{
		int dotPos = fileName.find_last_of('.');
		if (dotPos != -1)
		{
			return fileName.substr(dotPos + 1, fileName.length() - dotPos - 1);
		}
		return fileName;
	}

	// 首尾交换一段内存,width是内存方阵的宽度,每排的内存块数量,height是字节高度,blockByte是每块内存的字节数
	static void swapPixelBuffer(char* buffer, int width, int height, int blockByte);
	static void upsideDownBuffer(char* buffer, int bufferWidth, int startX, int startY, int width, int height, int blockByte);
	// YUV转RGBA,范围都是0-255
	static void YUVToRGBA(char* YUVData, int width, int height, char* RGBAData);
	// RGNA转YUV,范围都是0-255
	static void RGBAToYUV(char* RGBAData, int width, int height, char* YUVData);
	// YUV420P转换为RGB24
	static void YUV420PToRGB32(unsigned char* yuv420[3], unsigned char* rgb32, int width, int height);
	static void limitWithin0To255(int& value)
	{
		if (value < 0)
		{
			value = 0;
		}
		else if (value > 255)
		{
			value = 255;
		}
	}

	static void split(std::string str, const char * deli, std::vector<std::string> *vec);

	static std::string boolToString(bool value) { return value ? "True" : "False"; }
	static bool stringToBool(std::string str) { return str == "True" || str == "true"; }

	static std::string strReplace(std::string str, int begin, int end, std::string reStr);
	static float calculateFloat(std::string str);	// 以浮点数的计算法则计算一个表达式,只支持加减乘除和括号
	static int calculateInt(std::string str);		// 以整数的计算法则计算一个表达式,支持取余,加减乘除和括号
	static std::string intToString(int i, int limitLen = 0); // limitLen是字符串的最小长度,如果整数的位数不足最小长度,则会在前面加0
	static int stringToInt(std::string str)
	{
		return atoi(str.c_str());
	}
	static float pow_f(float f, int p);
	static std::string floatToString(float f, int precision);	//precision -> 精度,保留的小数的位数
	static float stringToFloat(std::string str)
	{
		return (float)atof(str.c_str());
	}

	static float randomFloat(float minFloat, float maxFloat)
	{
		float percent = (rand() % (1000 + 1)) / 1000.0f;
		return percent * (maxFloat - minFloat) + minFloat;
	}
	static int randomInt(int minInt, int maxInt)
	{
		return rand() % (maxInt - minInt + 1) + minInt;
	}

	// 判断oriString是否以pattern结尾
	static bool endWith(std::string oriString, std::string pattern)
	{
		if (oriString.length() < pattern.length())
		{
			return false;
		}
		std::string endString = oriString.substr(oriString.length() - pattern.length(), pattern.length());
		return endString == pattern;
	}
	// 判断oriString是否以pattern开头
	static bool startWith(std::string oriString, std::string pattern)
	{
		if (oriString.length() < pattern.length())
		{
			return false;
		}
		std::string startString = oriString.substr(0, pattern.length());
		return startString == pattern;
	}

	// 计算 16进制的c中1的个数
	static int crc_check(char c)
	{
		int count = 0;
		int bitCount = sizeof(char) * 8;
		for (int i = 0; i < bitCount; ++i)
		{
			if ((c & (0x01 << i)) > 0)
			{
				++count;
			}
		}
		return count;
	}

	// 秒数转换为分数和秒数
	static void secondsToMinutesSeconds(int seconds, int& outMin, int& outSec)
	{
		outMin = seconds / 60;
		outSec = seconds - outMin * 60;
	}

	static void secondsToHoursMinutesSeconds(int seconds, int& outHour, int& outMin, int& outSec)
	{
		outHour = seconds / (60 * 60);
		outMin = (seconds - outHour * (60 * 60)) / 60;
		outSec = seconds - outHour * (60 * 60) - outMin * 60;
	}

	static std::wstring ANSIToUnicode(const std::string& str);
	static std::string UnicodeToANSI(const std::wstring& str);
	static std::string UnicodeToUTF8(const std::wstring& str);
	static std::wstring UTF8ToUnicode(const std::string& str);
	static std::string ANSIToUTF8(const std::string& str);
	static std::string UTF8ToANSI(const std::string& str);

	// 将sourceFile拷贝到destFile,sourceFile和destFile都是带可直接访问的路径的文件名,overWrite指定当目标文件已经存在时是否要覆盖文件
	static bool copyFile(std::string sourceFile, std::string destFile, bool overWrite = true);
	// 创建一个文件夹,path是一个不以/结尾的可直接访问的相对或者绝对的文件夹名
	static bool createFolder(std::string path);
	static bool writeFile(std::string filePath, int length, const char* buffer);
	static void rightToLeft(std::string& str)
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
	static bool isFloatZero(float value, float precision = 0.0001f)
	{
		return value >= -precision && value <= precision;
	}
	static bool isFloatEqual(float value1, float value2, float precision = 0.0001f)
	{
		return isFloatZero(value1 - value2, precision);
	}
	static int strFind(std::string res, std::string sub, int startPose, bool fromStart);
	static bool findSubstr(std::string res, std::string dst, bool sensitive, int* pos = NULL, int startPose = 0, bool firstOrLast = true);
	
	template<typename T>
	static std::vector<std::string> findSubstrT(std::map<std::string, T>& res, std::string dst, bool sensitive)
	{
		std::vector <std::string> retList;
		// 循环遍历,如果匹配到放入列表
		typename std::map<std::string, T>::iterator itr = res.begin();
		typename std::map<std::string, T>::iterator itrEnd = res.end();
		for (; itr != itrEnd; ++itr)
		{
			std::string name = itr->first;
			if (txUtility::findSubstr(name, dst, sensitive))
			{
				retList.push_back(name);
			}
		}

		return retList;
	}

	static std::vector<std::string> findSubstr(std::vector<std::string>& res, std::string dst, bool sensitive)
	{
		std::vector <std::string> retList;
		std::vector<std::string>::size_type listSize = res.size();
		for (std::vector<std::string>::size_type i = 0; i < listSize; ++i)
		{
			if (txUtility::findSubstr(res[i], dst, sensitive))
			{
				retList.push_back(res[i]);
			}
		}

		return retList;
	}
	static void strToLower(std::string& str);
	static void strToUpper(std::string& str);

	// json
	static void jsonStartArray(std::string& str){ str += "["; }
	static void jsonEndArray(std::string& str)
	{ 
		removeLastComma(str);
		str += "],"; 
	}
	static void jsonStartStruct(std::string& str){ str += "{"; }
	static void jsonEndStruct(std::string& str)
	{
		removeLastComma(str);
		str += "},";
	}
	static void jsonAddPair(std::string& str, std::string name, std::string value)
	{
		str += "\"";
		str += name;
		str += "\":";
		str += "\"";
		str += value;
		str += "\",";
	}
	static std::string charToHex(unsigned char byte, bool upper = true);
	static std::string charArrayToHexString(unsigned char* data, const int& dataCount, bool addSpace = true, bool upper = true);
	template<typename T>
	static void clamp(T& value, T min, T max)
	{
		if (min >= max)
		{
			min = max;
		}
		if (value < min)
		{
			value = min;
		}
		else if (value > max)
		{
			value = max;
		}
	}
};

#endif
