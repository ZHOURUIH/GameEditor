#include "txSerializer.h"
#include "txEngineRoot.h"
#include "txEngineLog.h"

txSerializer::txSerializer(const bool& traceMemery)
:
mTraceMemery(traceMemery),
mWriteFlag(true),
mIndex(0),
mBufferSize(0),
mBuffer(NULL)
{}

txSerializer::txSerializer(char* buffer, const int& bufferSize, const bool& traceMemery)
:
mTraceMemery(traceMemery),
mWriteFlag(false),
mIndex(0),
mBufferSize(bufferSize),
mBuffer(buffer)
{}

txSerializer::~txSerializer()
{
	if (mWriteFlag)
	{
		if (mTraceMemery)
		{
			TRACE_DELETE_ARRAY(mBuffer);
		}
		else
		{
			NORMAL_DELETE_ARRAY(mBuffer);
		}
	}
}

void txSerializer::writeBuffer(char* buffer, const int& bufferSize)
{
	if (!writeCheck(bufferSize))
	{
		return;
	}
	memcpy(mBuffer + mIndex, buffer, bufferSize);
	mIndex += bufferSize;
}

void txSerializer::readBuffer(char* buffer, const int& bufferSize, const int& readLen)
{
	if (!readCheck(readLen))
	{
		return;
	}
	// 如果存放数据的空间大小不足以放入当前要读取的数据,则只拷贝能容纳的长度,但是下标应该正常跳转
	if (bufferSize <= readLen)
	{
		memcpy(buffer, mBuffer + mIndex, bufferSize);
		mIndex += readLen;
	}
	else
	{
		memcpy(buffer, mBuffer + mIndex, readLen);
		mIndex += readLen;
	}
}

void txSerializer::writeString(const char* str)
{
	int strLen = strlen(str);
	if (!writeCheck(strLen + sizeof(int)))
	{
		return;
	}
	// 先写入字符串长度
	write(strLen);
	memcpy(mBuffer + mIndex, str, strLen);
	mIndex += strLen;
}

void txSerializer::readString(char* str, const int& strBufferSize)
{
	if (!readCheck(sizeof(int)))
	{
		return;
	}
	// 先读入字符串长度
	int readLen = 0;
	read(readLen);
	if (!readCheck(readLen))
	{
		return;
	}
	// 如果存放字符串的空间大小不足以放入当前要读取的字符串,则只拷贝能容纳的长度,但是下标应该正常跳转
	if (strBufferSize <= readLen)
	{
		memcpy(str, mBuffer + mIndex, strBufferSize - 1);
		mIndex += readLen;
		// 加上结束符
		str[strBufferSize - 1] = 0;
	}
	else
	{
		memcpy(str, mBuffer + mIndex, readLen);
		mIndex += readLen;
		// 加上结束符
		str[readLen] = 0;
	}
}

void txSerializer::writeVector2(const VECTOR2& value)
{
	int writeLen = 2 * sizeof(value[0]);
	if (!writeCheck(writeLen))
	{
		return;
	}
	memcpy(mBuffer + mIndex, txMath::toPtr(value), writeLen);
	mIndex += writeLen;
}

void txSerializer::readVector2(VECTOR2& value)
{
	int readLen = 2 * sizeof(value[0]);
	if (!readCheck(readLen))
	{
		return;
	}
	memcpy((void*)txMath::toPtr(value), mBuffer + mIndex, readLen);
	mIndex += readLen;
}

void txSerializer::writeVector3(const VECTOR3& value)
{
	int writeLen = 3 * sizeof(value[0]);
	if (!writeCheck(writeLen))
	{
		return;
	}
	memcpy(mBuffer + mIndex, txMath::toPtr(value), writeLen);
	mIndex += writeLen;
}

void txSerializer::readVector3(VECTOR3& value)
{
	int readLen = 3 * sizeof(value[0]);
	if (!readCheck(readLen))
	{
		return;
	}
	memcpy((void*)txMath::toPtr(value), mBuffer + mIndex, readLen);
	mIndex += readLen;
}

void txSerializer::writeMatrix3(const MATRIX3& value)
{
	int writeLen = 9 * sizeof(value[0]);
	if (!writeCheck(writeLen))
	{
		return;
	}
	memcpy(mBuffer + mIndex, txMath::toPtr(value), writeLen);
	mIndex += writeLen;
}

void txSerializer::readMatrix3(MATRIX3& value)
{
	int readLen = 9 * sizeof(value[0]);
	if (!readCheck(readLen))
	{
		return;
	}
	memcpy((void*)txMath::toPtr(value), mBuffer + mIndex, readLen);
	mIndex += readLen;
}

void txSerializer::writeMatrix4(const MATRIX4& value)
{
	int writeLen = 16 * sizeof(value[0]);
	if (!writeCheck(writeLen))
	{
		return;
	}
	memcpy(mBuffer + mIndex, txMath::toPtr(value), writeLen);
	mIndex += writeLen;
}

void txSerializer::readMatrix4(MATRIX4& value)
{
	int readLen = 16 * sizeof(value[0]);
	if (!readCheck(readLen))
	{
		return;
	}
	memcpy((void*)txMath::toPtr(value), mBuffer + mIndex, readLen);
	mIndex += readLen;
}

void txSerializer::writeQuaternion(const QUATERNION& value)
{
	int writeLen = 4 * sizeof(value.x);
	if (!writeCheck(writeLen))
	{
		return;
	}
	memcpy(mBuffer + mIndex, txMath::toPtr(value), writeLen);
	mIndex += writeLen;
}

void txSerializer::readQuaternion(QUATERNION& value)
{
	int readLen = 4 * sizeof(value.x);
	if (!readCheck(readLen))
	{
		return;
	}
	memcpy((void*)txMath::toPtr(value), mBuffer + mIndex, readLen);
	mIndex += readLen;
}

void txSerializer::writeToFile(const std::string& fullName)
{
	// 确保是只写的,并且数据不为空
	if (!mWriteFlag || mBuffer == NULL || mIndex <= 0)
	{
		return;
	}
	txFileUtility::writeFile(fullName, mIndex, mBuffer);
}