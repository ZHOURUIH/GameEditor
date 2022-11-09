#pragma once

#include "CommonMacro.h"
#include "Array.h"

class Utility
{
public:
	static void makeSockAddr(sockaddr_in& addr, const ulong hostlong, const ushort port);
	static string getInternetIP(MY_SOCKET socket);
	static string getLocalIP(MY_SOCKET socket);
	template<typename T>
	static constexpr bool read(const char* buffer, const int bufferSize, int& index, T& value)
	{
		constexpr int valueSize = sizeof(T);
		if (bufferSize < index + valueSize)
		{
			return false;
		}
		value = *((T*)(buffer + index));
		index += valueSize;
		return true;
	}
	template<typename T>
	static constexpr T read(const char* buffer, const int bufferSize, int& index)
	{
		constexpr int valueSize = sizeof(T);
		if (bufferSize < index + valueSize)
		{
			return (T)0;
		}
		const T value = *((T*)(buffer + index));
		index += valueSize;
		return value;
	}
	template<typename T>
	static constexpr bool write(char* buffer, const int bufferSize, int& index, const T value)
	{
		constexpr int writeSize = sizeof(T);
		if (bufferSize < index + writeSize)
		{
			return false;
		}
		*((T*)(buffer + index)) = value;
		index += writeSize;
		return true;
	}
	template<typename T>
	static constexpr void clampMin(T& value, const T minValue = (T)0)
	{
		if (value < minValue)
		{
			value = minValue;
		}
	}
	static llong getRealTimeMS();
	static void sleep(ulong timeMS);
	// 返回string类型的数字字符串,速度较慢,limitLen是字符串的最小长度,如果整数的位数不足最小长度,则会在前面加0
	static string intToString(const int value, const int limitLen = 0);
	template<size_t Length>
	static int intToString(Array<Length>& charArray, const int value, const int limitLen = 0)
	{
		if (limitLen == 0)
		{
			return _itoa_s(value, charArray);
		}
		// 因为当前函数设计为线程安全,所以只能使用栈内存中的数组
		Array<16> temp{ 0 };
		const int len = _itoa_s(value, temp);
		// 判断是否需要在前面补0
		if (limitLen > len)
		{
			// 因为当前函数设计为线程安全,所以只能使用栈内存中的数组
			Array<16> zeroArray{ 0 };
			zeroString(zeroArray, limitLen - len);
			strcat_t(charArray, zeroArray.str(), temp.str());
			charArray[limitLen] = '\0';
			return limitLen;
		}
		else
		{
			charArray.copy(temp.str(), len);
			charArray[len] = '\0';
			return len;
		}
	}
	static int getThreadID()
	{
#ifdef WIN32
		return (int)GetCurrentThreadId();
#else
		return (int)pthread_self();
#endif
	}
	template<size_t Length>
	static int _itoa_s(int value, Array<Length>& charArray)
	{
		if (value == 0)
		{
			charArray[0] = '0';
			charArray[1] = '\0';
			return 1;
		}

		int sign = 1;
		if (value < 0)
		{
			value = -value;
			sign = -1;
		}
		int index = 0;
		while (true)
		{
			// 如果是正数,则数字个数不能超过size - 1
			// 如果是负数,则数字个数不能超过size - 2
			if ((sign > 0 && index >= (int)Length) ||
				(sign < 0 && index >= (int)Length - 1))
			{
				ERROR("buffer is too small!");
				break;
			}
			// 将数字放入numberArray的尾部
			if ((llong)value < POWER_INT_10[index])
			{
				break;
			}
			charArray[Length - 1 - index] = (int)((llong)value % POWER_INT_10[index + 1] / POWER_INT_10[index]);
			++index;
		}
		// 将数字从数组末尾移动到开头,并且将数字转换为数字字符
		if (sign > 0)
		{
			const int lengthToHead = Length - index;
			for (int i = 0; i < index; ++i)
			{
				charArray[i] = charArray[i + lengthToHead] + '0';
			}
			charArray[index] = '\0';
		}
		else
		{
			charArray[0] = '-';
			const int lengthToHead = Length - index;
			for (int i = 0; i < index; ++i)
			{
				charArray[i + 1] = charArray[i + lengthToHead] + '0';
			}
			charArray[++index] = '\0';
		}
		return index;
	}
	static string zeroString(const int zeroCount);
	template<typename T>
	static constexpr void swap(T& value0, T& value1)
	{
		const T temp = value0;
		value0 = value1;
		value1 = temp;
	}
	static string getTime(const bool timeStamp = false);
protected:
	static const Array<11, llong> POWER_INT_10;
};