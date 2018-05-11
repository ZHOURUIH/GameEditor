#ifndef _TX_SERIALIZER_H_
#define _TX_SERIALIZER_H_

#include "EditorCoreLog.h"
#include "CommonDefine.h"

// 用于生成二进制文件的
class txSerializer
{
public:
	txSerializer(bool traceMemery = true);
	txSerializer(char* buffer, int bufferSize);
	~txSerializer();
	template<typename T>
	void write(T value, bool inverse = false)
	{
		// 如果是只读的,则不能写入
		if (!mWriteFlag)
		{
			EDITOR_CORE_ERROR("error : the buffer is read only, can not write!");
			return;
		}
		int writeLen = sizeof(T);
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
		BinaryUtility::write(value, (unsigned char*)mBuffer, mIndex, inverse);
	}
	template<typename T>
	void read(T& value, bool inverse = false)
	{
		// 如果是只写的,则不能读取
		if (mWriteFlag)
		{
			EDITOR_CORE_ERROR("error : the buffer is write only, can not read!");
			return;
		}
		if (mBuffer == NULL)
		{
			EDITOR_CORE_ERROR("error : buffer is NULL! can not read");
			return;
		}
		int readLen = sizeof(T);
		if (mIndex + readLen > mBufferSize)
		{
			EDITOR_CORE_ERROR("error : read buffer out of range! cur index : %d, buffer size : %d, read length : %d", mIndex, mBufferSize, readLen);
			return;
		}
		BinaryUtility::read(value, (unsigned char*)mBuffer, mIndex, inverse);
	}
	void writeBuffer(char* buffer, int bufferSize);
	void readBuffer(char* buffer, int bufferSize, int readLen);
	void writeString(const char* str);
	void readString(char* str, int strBufferSize);
	void writeToFile(std::string fullName);
	const char* getBuffer() const { return mBuffer; }
	int getBufferSize() { return mBufferSize; }
	int getDataSize() { return mIndex; }
protected:
	void resizeBuffer(int maxSize)
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
	void createBuffer(int bufferSize)
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