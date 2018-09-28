#ifndef _BINARY_UTILITY_H_
#define _BINARY_UTILITY_H_

#include "ServerDefine.h"

class BinaryUtility
{
public:
	static unsigned short crc16_table[256];
	static void initCRCTable();
	// 计算 16进制的c中1的个数
	static int crc_check(char c);
	static unsigned short crc16(unsigned short crc, char* buffer, int len, int bufferOffset = 0);
	static unsigned short crc16_byte(unsigned short crc, unsigned char data);
	static bool readBuffer(char* buffer, int bufferSize, int& index, char* dest, int readSize);
	static bool writeBuffer(char* buffer, int bufferSize, int& destOffset, char* source, int writeSize);
	template<typename T>
	static T read(char* buffer, int bufferSize, int& index, bool inverse = false)
	{
		int valueSize = sizeof(T);
		if (bufferSize < index + valueSize)
		{
			return 0;
		}
		txVector<int> bytes;
		for (int i = 0; i < valueSize; ++i)
		{
			char c = buffer[index++];
			bytes.push_back((int)(0xFF & c));
		}
		T finalValue = 0;
		if (inverse)
		{
			for (int i = 0; i < valueSize; ++i)
			{
				finalValue |= (bytes[i] << (8 * (valueSize - 1 - i)));
			}
		}
		else
		{
			for (int i = 0; i < valueSize; ++i)
			{
				finalValue |= (bytes[i] << (8 * i));
			}
		}
		return finalValue;
	}
	template<typename T>
	static void readArray(char* buffer, int bufferSize, int& index, T* dest, int arrayLength, bool inverse = false)
	{
		for (int i = 0; i < arrayLength; ++i)
		{
			dest[i] = read<T>(buffer, bufferSize, index, inverse);
		}
	}
	template<typename T>
	static bool write(char* buffer, int bufferSize, int& index, T value, bool inverse = false)
	{
		int writeSize = sizeof(T);
		if (bufferSize < index + writeSize)
		{
			return false;
		}
		if (inverse)
		{
			for (int i = 0; i < writeSize; ++i)
			{
				buffer[index++] = ((0xFF << ((writeSize - 1 - i) * 8)) & value) >> ((writeSize - 1 - i) * 8);
			}
		}
		else
		{
			for (int i = 0; i < writeSize; ++i)
			{
				buffer[index++] = ((0xFF << (i * 8)) & value) >> (i * 8);
			}
		}
		return true;
	}
	template<typename T>
	static bool writeArray(char* buffer, int bufferSize, int& index, T* valueArray, int arrayLength, bool inverse = false)
	{
		for (int i = 0; i < arrayLength; ++i)
		{
			if (!write(buffer, bufferSize, index, valueArray[i], inverse))
			{
				return false;
			}
		}
		return true;
	}
	template<typename T>
	static void inverseByte(T& value)
	{
		int typeSize = sizeof(T);
		for (int i = 0; i < typeSize / 2; ++i)
		{
			swapByte(value, i, typeSize - i - 1);
		}
	}
	template<typename T>
	static void swapByte(T& value, int pos0, int pos1)
	{
		char byte0 = (value & (0xff << (8 * pos0))) >> (8 * pos0);
		char byte1 = (value & (0xff << (8 * pos1))) >> (8 * pos1);
		SET_BYTE(value, byte0, pos1);
		SET_BYTE(value, byte1, pos0);
	}
};

#endif