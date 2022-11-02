#pragma once

#include "CommonMacro.h"

class Utility
{
public:
	static void makeSockAddr(sockaddr_in& addr, const ulong hostlong, const ushort port);
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
};