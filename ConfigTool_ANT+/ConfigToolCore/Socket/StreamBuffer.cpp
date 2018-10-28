#include "StreamBuffer.h"
#include "Utility.h"
#include "txMemoryTrace.h"
#include "ToolCoreLog.h"
#include "ToolCoreUtility.h"

void StreamBuffer::resizeBuffer(const int& size)
{
	if (mBufferSize >= size)
	{
		return;
	}
	if (mReadBuffer != NULL)
	{
		// 创建新的缓冲区,将原来的数据拷贝到新缓冲区中,销毁原缓冲区,指向新缓冲区
		unsigned char* newBuffer = TRACE_NEW_ARRAY(unsigned char, size, newBuffer);
		if (mDataLength > 0)
		{
			memcpy(newBuffer, mReadBuffer, mDataLength);
		}
		TRACE_DELETE_ARRAY(mReadBuffer);
		mReadBuffer = newBuffer;
		mBufferSize = size;
	}
	else
	{
		mReadBuffer = TRACE_NEW_ARRAY(unsigned char, size, mReadBuffer);
		mBufferSize = size;
	}
}

void StreamBuffer::addDataToBuffer(unsigned char* data, const int& dataCount)
{
	// 如果当前已经存放不下新的数据了,不再处理新数据
	if (mBufferSize - mDataLength < dataCount)
	{
		return;
	}
	memcpy(mReadBuffer + mDataLength, data, dataCount);
	mDataLength += dataCount;
}

void StreamBuffer::removeDataFromBuffer(const int& start, const int& count)
{
	if (mDataLength < start + count)
	{
		return;
	}
	memmove(mReadBuffer + start, mReadBuffer + start + count, mDataLength - start - count);
	mDataLength -= count;
}

void StreamBuffer::clearBuffer()
{
	mDataLength = 0;
}