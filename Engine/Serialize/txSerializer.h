#ifndef _TX_SERIALIZER_H_
#define _TX_SERIALIZER_H_

#include "txEngineLog.h"
#include "Utility.h"

// 用于生成二进制文件的
class txSerializer
{
public:
	txSerializer(const bool& traceMemery = true);
	txSerializer(char* buffer, const int& bufferSize, const bool& traceMemery = true);
	virtual ~txSerializer();
	template<typename T>
	void write(const T& value)
	{
		int writeLen = sizeof(T);
		if (!writeCheck(writeLen))
		{
			return;
		}
		memcpy(mBuffer + mIndex, &value, writeLen);
		mIndex += writeLen;
	}
	template<typename T>
	void read(T& value)
	{
		int readLen = sizeof(T);
		if (!readCheck(readLen))
		{
			return;
		}
		value = *((T*)(mBuffer + mIndex));
		mIndex += readLen;
	}
	void writeBuffer(char* buffer, const int& bufferSize);
	void readBuffer(char* buffer, const int& bufferSize, const int& readLen);
	void writeString(const char* str);
	void readString(char* str, const int& strBufferSize);
	void writeVector2(const VECTOR2& value);
	void readVector2(VECTOR2& value);
	void writeVector3(const VECTOR3& value);
	void readVector3(VECTOR3& value);
	void writeMatrix3(const MATRIX3& value);
	void readMatrix3(MATRIX3& value);
	void writeMatrix4(const MATRIX4& value);
	void readMatrix4(MATRIX4& value);
	void writeQuaternion(const QUATERNION& value);
	void readQuaternion(QUATERNION& value);
	void writeToFile(const std::string& fullName);
	const char* getBuffer() const { return mBuffer; }
	const int& getBufferSize() { return mBufferSize; }
	const int& getDataSize() { return mIndex; }
protected:
	bool writeCheck(const int& writeLen)
	{
		// 如果是只读的,则不能写入
		if (!mWriteFlag)
		{
			ENGINE_ERROR("error : the buffer is read only, can not write!");
			return false;
		}
		// 如果缓冲区为空,则创建缓冲区
		if (mBuffer == NULL)
		{
			createBuffer(writeLen);
		}
		// 如果缓冲区已经不够用了,则重新扩展缓冲区
		else if (writeLen + mIndex > mBufferSize)
		{
			resizeBuffer(writeLen + mIndex);
		}
		return true;
	}
	bool readCheck(const int& readLen)
	{
		// 如果是只写的,则不能读取
		if (mWriteFlag)
		{
			ENGINE_ERROR("error : the buffer is write only, can not read!");
			return false;
		}
		if (mBuffer == NULL)
		{
			ENGINE_ERROR("error : buffer is NULL! can not read");
			return  false;
		}
		if (mIndex + readLen > mBufferSize)
		{
			ENGINE_ERROR("error : read buffer out of range! cur index : %d, buffer size : %d, read length : %d", mIndex, mBufferSize, readLen);
			return  false;
		}
		return true;
	}
	void resizeBuffer(const int& maxSize)
	{
		int newSize = maxSize > mBufferSize * 2 ? maxSize : mBufferSize * 2;
		char* newBuffer = NULL;
		if (mTraceMemery)
		{
			newBuffer = TRACE_NEW_ARRAY(char, newSize, newBuffer);
		}
		else
		{
			newBuffer = NORMAL_NEW_ARRAY(char, newSize, newBuffer);
		}
		memcpy(newBuffer, mBuffer, mBufferSize);
		if (mTraceMemery)
		{
			TRACE_DELETE_ARRAY(mBuffer);
		}
		else
		{
			NORMAL_DELETE_ARRAY(mBuffer);
		}
		mBuffer = newBuffer;
		mBufferSize = newSize;
	}
	void createBuffer(const int& bufferSize)
	{
		if (mBuffer == NULL)
		{
			mBufferSize = bufferSize;
			if (mTraceMemery)
			{
				mBuffer = TRACE_NEW_ARRAY(char, mBufferSize, mBuffer);
			}
			else
			{
				mBuffer = NORMAL_NEW_ARRAY(char, mBufferSize, mBuffer);
			}
		}
	}
protected:
	char* mBuffer;
	int mIndex;
	int mBufferSize;
	bool mWriteFlag;	// 如果为真,则表示是只写的,为假则表示是只读的
	bool mTraceMemery;	// 为真则表示所有的内存申请释放操作都会被记录下来,为假则不会被记录,在记录内存跟踪信息时会用到
};

#endif