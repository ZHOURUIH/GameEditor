#ifndef _ARRAY_H_
#define _ARRAY_H_

#include <string>
#include "IndependentLog.h"
#include "CommonMacro.h"

// 为了能够给T设置默认参数,只能将T放在最后
// 由于char数组是使用频率最高的类型,所以默认参数为char,也就是当使用Array<256>时就表示Array<256, char>
template<size_t Length, class T = char>
class Array
{
public:
	const T* data() const { return mValue; }
	T* data() { return mValue; }
	int eraseElement(const T& element, const int curCount)
	{
		int newCount = curCount;
		FOR_INVERSE_I(curCount)
		{
			if (mValue[i] == element)
			{
				memmove((void*)(mValue + i), (void*)(mValue + i + 1), sizeof(T) * ((int)Length - i - 1));
				--newCount;
			}
		}
		return newCount;
	}
	int eraseLastElement(const T& element, const int curCount)
	{
		int newCount = curCount;
		FOR_INVERSE_I(curCount)
		{
			if (mValue[i] == element)
			{
				memmove((void*)(mValue + i), (void*)(mValue + i + 1), sizeof(T) * ((int)Length - i - 1));
				--newCount;
				break;
			}
		}
		return newCount;
	}
	int eraseFirstElement(const T& element, const int curCount)
	{
		int newCount = curCount;
		FOR_I(curCount)
		{
			if (mValue[i] == element)
			{
				memmove((void*)(mValue + i), (void*)(mValue + i + 1), sizeof(T) * ((int)Length - i - 1));
				--newCount;
				break;
			}
		}
		return newCount;
	}
	bool contains(const T& element, int length = -1)
	{
		if (length < 0)
		{
			length = (int)Length;
		}
		FOR_I(length)
		{
			if (mValue[i] == element)
			{
				return true;
			}
		}
		return false;
	}
	template<size_t SourceLength>
	void copy(const Array<SourceLength, T>& src)
	{
		if (SourceLength > (int)Length)
		{
			IndependentLog::directLog("拷贝数组太长");
			return;
		}
		MEMCPY(mValue, sizeof(T) * (int)Length, src.data(), sizeof(T) * SourceLength);
	}
	template<size_t SourceLength>
	void copy(const int destOffset, const Array<SourceLength, T>& src)
	{
		if (destOffset + SourceLength > (int)Length)
		{
			IndependentLog::directLog("拷贝数组太长");
			return;
		}
		MEMCPY(mValue + sizeof(T) * destOffset, sizeof(T) * ((int)Length - destOffset), src.data(), sizeof(T) * SourceLength);
	}
	template<size_t SourceLength>
	void copy(const Array<SourceLength, T>& src, const int copyCount)
	{
		if (copyCount > (int)Length)
		{
			IndependentLog::directLog("拷贝数组太长");
			return;
		}
		MEMCPY(mValue, sizeof(T) * (int)Length, src.data(), sizeof(T) * copyCount);
	}
	template<size_t SourceLength>
	void copy(const int destOffset, const Array<SourceLength, T>& src, const int copyCount)
	{
		if (destOffset + copyCount > (int)Length)
		{
			IndependentLog::directLog("拷贝数组太长");
			return;
		}
		MEMCPY(mValue + sizeof(T) * destOffset, sizeof(T) * ((int)Length - destOffset), src.data(), sizeof(T) * copyCount);
	}
	void copy(const T* src, const int copyCount)
	{
		if (copyCount > (int)Length)
		{
			IndependentLog::directLog("拷贝数组太长");
			return;
		}
		MEMCPY(mValue, sizeof(T) * (int)Length, src, sizeof(T) * copyCount);
	}
	void copy(const int destOffset, const T* src, const int copyCount)
	{
		if (destOffset + copyCount > (int)Length)
		{
			IndependentLog::directLog("拷贝数组太长");
			return;
		}
		MEMCPY(mValue + sizeof(T) * destOffset, sizeof(T) * ((int)Length - destOffset), src, sizeof(T) * copyCount);
	}
	T* data(const int offset)
	{
		if (offset >= (int)Length)
		{
			IndependentLog::directError("数组越界");
			return nullptr;
		}
		return mValue + offset; 
	}
	const T* data(const int offset) const
	{
		if (offset >= (int)Length)
		{
			IndependentLog::directError("数组越界");
			return nullptr;
		}
		return mValue + offset;
	}
	constexpr int size() const { return (int)Length; }
	constexpr const T& operator[](int index) const
	{
		if (index < 0 || index >= (int)Length)
		{
			IndependentLog::directError("数组越界");
			index = (int)Length - 1;
		}
		return mValue[index];
	}
	constexpr T& operator[](int index)
	{
		if (index < 0 || index >= (int)Length)
		{
			IndependentLog::directError("数组越界");
			index = (int)Length - 1;
		}
		return mValue[index];
	}
	// 将数组的内容重置为0,但是需要外部确保将所有字节设置为0不会引起问题,如果T类型是string或者其他的类则不允许使用此函数
	// 因为那会将虚表也重置为0,引起崩溃
	void zero()
	{
		memset(mValue, 0, sizeof(T) * (int)Length);
	}
	// 将数组的每一个元素都设置为value
	void fillArray(const T& value)
	{
		FOR_I((int)Length)
		{
			mValue[i] = value;
		}
	}
	void fillArray(const int start, const T& value)
	{
		for (int i = start; i < (int)Length; ++i)
		{
			mValue[i] = value;
		}
	}
public:
	T mValue[(int)Length];
};

// 为长度为0特化的类型
template<typename T>
class Array<0, T>
{
public:
	T mValue[1];
};

// 为char特化的类型,因为常常会当作字符串使用
template<size_t Length>
class Array<Length, char>
{
public:
	// 为了方便查代码,将返回指针的函数命名为具体的名字
	const char* str() const			{ return mValue; }
	char* toBuffer()				{ return mValue; }
	// 仅为了在类型匹配时能够编译通过,尽量避免直接使用
	const char* data() const		{ return mValue; }
	char* data()					{ return mValue; }
	int length() const 
	{
		FOR_I(Length)
		{
			if (mValue[i] == '\0')
			{
				return i;
			}
		}
		return Length;
	}
	template<size_t SourceLength>
	void copy(const Array<SourceLength>& src)
	{
		if (SourceLength > (int)Length)
		{
			IndependentLog::directLog("拷贝字符串太长");
			return;
		}
		MEMCPY(mValue, (int)Length, src.str(), SourceLength);
	}
	template<size_t SourceLength>
	void copy(const Array<SourceLength>& src, const int copyCount)
	{
		if (copyCount > (int)Length)
		{
			IndependentLog::directLog("拷贝字符串太长");
			return;
		}
		MEMCPY(mValue, (int)Length, src.str(), copyCount);
	}
	void copy(const char* src) const
	{
		const int length = (int)::strlen(src);
		if (length > (int)Length)
		{
			IndependentLog::directLog("拷贝字符串太长");
			return;
		}
		MEMCPY(mValue, (int)Length, src, ::strlen(src));
	}
	void copy(const char* src, const int copyCount) const
	{
		if (copyCount > (int)Length)
		{
			IndependentLog::directLog("拷贝字符串太长");
			return;
		}
		MEMCPY(mValue, (int)Length, src, copyCount);
	}
	void copy(const string& src, const int copyCount) const
	{
		if (copyCount > (int)Length)
		{
			IndependentLog::directLog("拷贝字符串太长");
			return;
		}
		MEMCPY(mValue, (int)Length, src.c_str(), copyCount);
	}
	void copy(const string& src, const int srcOffset, const int copyCount) const
	{
		if (copyCount > (int)Length)
		{
			IndependentLog::directLog("拷贝字符串太长");
			return;
		}
		MEMCPY(mValue, (int)Length, src.c_str() + srcOffset, copyCount);
	}
	template<size_t SourceLength>
	void copy(const int destOffset, const Array<SourceLength>& src)
	{
		if (SourceLength + destOffset > (int)Length)
		{
			IndependentLog::directLog("拷贝字符串太长");
			return;
		}
		MEMCPY(mValue + destOffset, (int)Length - destOffset, src.str(), SourceLength);
	}
	template<size_t SourceLength>
	void copy(const int destOffset, const Array<SourceLength>& src, const int copyCount)
	{
		if (copyCount + destOffset > (int)Length)
		{
			IndependentLog::directLog("拷贝字符串太长");
			return;
		}
		MEMCPY(mValue + destOffset, (int)Length - destOffset, src.str(), copyCount);
	}
	void copy(const int destOffset, const char* src, const int copyCount)
	{
		if (copyCount + destOffset > (int)Length)
		{
			IndependentLog::directLog("拷贝字符串太长");
			return;
		}
		MEMCPY(mValue + destOffset, (int)Length - destOffset, src, copyCount);
	}
	void copy(const int destOffset, const string& src)
	{
		if (destOffset + src.length() > (int)Length)
		{
			IndependentLog::directLog("拷贝字符串太长");
			return;
		}
		MEMCPY(mValue + destOffset, (int)Length - destOffset, src.c_str(), src.length());
	}
	void copy(const int destOffset, const string& src, const int copyLength)
	{
		if (destOffset + copyLength > (int)Length)
		{
			IndependentLog::directLog("拷贝字符串太长");
			return;
		}
		MEMCPY(mValue + destOffset, (int)Length - destOffset, src.c_str(), copyLength);
	}
	constexpr int size() const { return (int)Length; }
	constexpr const char& operator[](int index) const
	{
		if (index < 0 || index >= (int)Length)
		{
			IndependentLog::directError("数组越界");
			index = (int)Length - 1;
		}
		return mValue[index];
	}
	constexpr char& operator[](int index)
	{
		if (index < 0 || index >= (int)Length)
		{
			IndependentLog::directError("数组越界");
			index = (int)Length - 1;
		}
		return mValue[index];
	}
	// 将数组的内容重置为0
	void zero()
	{
		memset(mValue, 0, sizeof(char) * (int)Length);
	}
	// 将数组的每一个元素都设置为value
	void fillArray(const char value)
	{
		memset(mValue, value, (int)Length);
	}
	// 将数组的从start开始的所有元素都设置为value
	void fillArray(const int start, const char value)
	{
		memset(mValue + start, value, (int)Length - start);
	}
	void setString(const char* str, int strLength = 0)
	{
		if (strLength == 0)
		{
			strLength = (int)::strlen(str);
		}
		copy(str, strLength);
		mValue[strLength] = '\0';
	}
	void setString(const string& str)
	{
		const int len = (int)str.length();
		copy(str.c_str(), len);
		mValue[len] = '\0';
	}
	void setString(int offset, const char* str, int strLength = 0)
	{
		if (strLength == 0)
		{
			strLength = (int)::strlen(str);
		}
		copy(offset, str, strLength);
		mValue[offset + strLength] = '\0';
	}
	void setString(const int offset, const string& str)
	{
		copy(offset, str);
		mValue[offset + (int)str.length()] = '\0';
	}
public:
	char mValue[(int)Length];
};

#endif