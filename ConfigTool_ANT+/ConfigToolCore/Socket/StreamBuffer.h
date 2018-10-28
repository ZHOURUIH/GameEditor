#ifndef _STREAM_BUFFER_H_
#define _STREAM_BUFFER_H_

#include "CommonDefine.h"

class StreamBuffer
{
public:
	void resizeBuffer(const int& size);
	void addDataToBuffer(unsigned char* data, const int& dataCount);
	void removeDataFromBuffer(const int& start, const int& count);
	void clearBuffer();
public:
	unsigned char*			mReadBuffer;			// 接收到的数据的缓冲区
	std::atomic<int>		mDataLength;			// 缓冲区中剩余接收到的数据长度
	std::atomic<int>		mBufferSize;			// 缓冲区总大小
};

#endif