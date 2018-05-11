#include "HeaderData.h"

PARSE_RESULT HeaderData::setData(unsigned char* data, const int& count)
{
	// 长度不够包头时,数据不足
	if (count < mHeaderLength)
	{
		return PR_NOT_ENOUGH;
	}
	// 确保一些固定数值是正确的
	if (data[0] != mMagicByte || data[1] != mVersion || data[mHeaderLength - 1] != mSeqID)
	{
		return PR_ERROR;
	}
	int offset = sizeof(mMagicByte) + sizeof(mVersion);
	unsigned short payloadLength = 0;
	BinaryUtility::read(payloadLength, data, offset, true);
	// 长度不够记录的数据长度,数据不足
	if (payloadLength > count - mHeaderLength)
	{
		return PR_NOT_ENOUGH;
	}
	unsigned short crc = 0;
	BinaryUtility::read(crc, data, offset, true);
	// 校验失败,数据错误
	unsigned short dataCRC16 = BinaryUtility::crc16(0xFF, data + mHeaderLength, payloadLength);
	if (crc != dataCRC16)
	{
		return PR_ERROR;
	}
	// 只有解析成功时,才保存数据
	mPayloadLength = payloadLength;
	mCRC16 = crc;
	return PR_SUCCESS;
}