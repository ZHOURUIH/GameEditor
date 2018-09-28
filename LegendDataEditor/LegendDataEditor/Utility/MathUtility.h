#ifndef _MATH_UTILITY_H_
#define _MATH_UTILITY_H_

#include "ServerDefine.h"

class MathUtility
{
public:
	static const float MATH_PI;
	static const float MIN_DELTA;
	static void checkInt(float& value, float precision = MIN_DELTA); // 判断传入的参数是否已经接近于整数,如果接近于整数,则设置为整数
	static bool isFloatZero(float value, float precision = MIN_DELTA){return value >= -precision && value <= precision;}
	static bool isFloatEqual(float value1, float value2, float precision = MIN_DELTA){return isFloatZero(value1 - value2, precision);}
	// 得到比value大的第一个pow的n次方的数
	static int getGreaterPowerValue(int value, int pow);
	// 得到数轴上浮点数右边的第一个整数,向上取整
	static int getForwardInt(float value);
	template<typename T>
	static void clamp(T& value, T minValue, T maxValue)
	{
		if (value > maxValue)
		{
			value = maxValue;
		}
		else if (value < minValue)
		{
			value = minValue;
		}
	}
	template<typename T>
	static void clampMin(T& value, T minValue)
	{
		if (value < minValue)
		{
			value = minValue;
		}
	}
	template<typename T>
	static void clampMax(T& value, T maxValue)
	{
		if (value > maxValue)
		{
			value = maxValue;
		}
	}
	static float randomFloat(float minFloat, float maxFloat)
	{
		float percent = (rand() % (1000 + 1)) / 1000.0f;
		return percent * (maxFloat - minFloat) + minFloat;
	}
	static int randomInt(int minInt, int maxInt)
	{
		if (minInt >= maxInt)
		{
			return minInt;
		}
		return rand() % (maxInt - minInt + 1) + minInt;
	}
	static float angleToRadian(float angle)
	{
		return angle * 3.14159f / 180.0f;
	}
	static float radianToAngle(float radian)
	{
		return radian * 180.0f / 3.14159f;
	}
	static void clampValue(float& value, float min, float max, float cycle)
	{
		while (value < min)
		{
			value += cycle;
		}
		while (value > max)
		{
			value -= cycle;
		}
	}
	static void clampAngle(float& angle, float min, float max, float pi)
	{
		clampValue(angle, min, max, pi * 2.0f);
	}
	static void adjustRadian180(float& radianAngle)
	{
		clampAngle(radianAngle, -MATH_PI, MATH_PI, MATH_PI);
	}
	static void adjustAngle180(float& radianAngle)
	{
		clampAngle(radianAngle, -180.0f, 180.0f, 180.0f);
	}
	static void adjustRadian360(float& radianAngle)
	{
		clampAngle(radianAngle, 0.0f, MATH_PI * 2.0f, MATH_PI);
	}
	static void adjustAngle360(float& radianAngle)
	{
		clampAngle(radianAngle, 0.0f, 360.0f, 180.0f);
	}
	static bool isInRange(int value, int range0, int range1)
	{
		return value >= getMin(range0, range1) && value <= getMax(range0, range1);
	}
	static bool isInRange(float value, float range0, float range1)
	{
		return value >= getMin(range0, range1) && value <= getMax(range0, range1);
	}
	template<typename T>
	static T getMin(T a, T b)
	{
		return a < b ? a : b;
	}
	template<typename T>
	static T getMax(T a, T b)
	{
		return a > b ? a : b;
	}
	template<typename T>
	static float lerpSimple(const T& start, const T& end, float t)
	{
		return start + (end - start) * t;
	}
	template<typename T>
	static float lerp(T start, T end, T t, T minAbsDelta = 0.0f)
	{
		clamp(t, 0.0f, 1.0f);
		float value = start + (end - start) * t;
		// 如果值已经在end的一定范围内了,则直接设置为end
		if (std::abs(value - end) <= minAbsDelta)
		{
			value = end;
		}
		return value;
	}
	static float inverseLerp(float a, float b, float value)
	{
		return (value - a) / (b - a);
	}
	// 将表达式str中的keyword替换为replaceValue,然后计算str的值,返回值表示str中是否有被替换的值,str只能是算术表达式
	static bool replaceKeywordAndCalculate(std::string& str, const std::string& keyword, int replaceValue, bool floatOrInt);
	// 将表达式str中的所有\\()包含的部分中的keyword替换为keyValue,并且计算包含的表达式,返回值表示str中是否有被替换的部分,str可以是任意表达式
	static bool replaceStringKeyword(std::string& str, const std::string& keyword, int keyValue, bool floatOrInt);
	static float powerFloat(float f, int p);
	static float calculateFloat(std::string str);	// 以浮点数的计算法则计算一个表达式,只支持加减乘除和括号
	static int calculateInt(std::string str);		// 以整数的计算法则计算一个表达式,支持取余,加减乘除和括号
	// 秒数转换为分数和秒数
	static void secondsToMinutesSeconds(int seconds, int& outMin, int& outSec);
	static void secondsToHoursMinutesSeconds(int seconds, int& outHour, int& outMin, int& outSec);
	template<typename T>
	static void swap(T& value0, T& value1)
	{
		T temp = value0;
		value0 = value1;
		value1 = temp;
	}
	template<typename T>
	static T getSign(T value)
	{
		if (value > (T)0)
		{
			return (T)1;
		}
		else if (value < (T)0)
		{
			return (T)-1;
		}
		else
		{
			return (T)0;
		}
	}
	float HueToRGB(float v1, float v2, float vH);
};

#endif