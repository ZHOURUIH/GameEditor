#ifndef _MATH_UTILITY_H_
#define _MATH_UTILITY_H_

#include "CommonDefine.h"

class MathUtility
{
public:
	static float calculateFloat(std::string str);	// 以浮点数的计算法则计算一个表达式,只支持加减乘除和括号
	static int calculateInt(std::string str);		// 以整数的计算法则计算一个表达式,支持取余,加减乘除和括号
	static float randomFloat(float minFloat, float maxFloat)
	{
		float percent = (rand() % (1000 + 1)) / 1000.0f;
		return percent * (maxFloat - minFloat) + minFloat;
	}
	static int randomInt(int minInt, int maxInt)
	{
		return rand() % (maxInt - minInt + 1) + minInt;
	}
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
	static bool isFloatZero(float value, float precision = 0.0001f)
	{
		return value >= -precision && value <= precision;
	}
	static bool isFloatEqual(float value1, float value2, float precision = 0.0001f)
	{
		return isFloatZero(value1 - value2, precision);
	}
};

#endif