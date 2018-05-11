#ifndef _HEADER_DATA_H_
#define _HEADER_DATA_H_

#include "CommonDefine.h"

// 数据包头
class HeaderData
{
public:
	HeaderData()
	{
		mReportID = REPORT_IN;
		mMagicByte = (char)0xa5;
		mVersion = 0;
		mPayloadLength = 0;
		mCRC16 = 0;
		mSeqID = 0;
		mHeaderLength = sizeof(mReportID)+sizeof(mMagicByte)+sizeof(mVersion)+sizeof(mPayloadLength)+sizeof(mCRC16)+sizeof(mSeqID);
	}
	virtual ~HeaderData(){}
	PARSE_RESULT setData(unsigned char* data, const int& count);
	int getHeaderLength() { return mHeaderLength; }
public:
	int mHeaderLength;				// 包头长度
	unsigned char mReportID;		// 输入的reportID，固定为0x02
	unsigned char mMagicByte;		// 固定0xa5
	unsigned char mVersion;			// 当前版本号,0
	unsigned short mPayloadLength;	// payload数据长度
	unsigned short mCRC16;			// payload数据CRC16校验结果
	unsigned short mSeqID;			// 保留字段,固定为0
};

#endif