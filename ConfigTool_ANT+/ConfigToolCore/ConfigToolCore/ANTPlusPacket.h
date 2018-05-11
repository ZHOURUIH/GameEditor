#ifndef _ANT_PLUS_PACKET_H_
#define _ANT_PLUS_PACKET_H_

#include "CommonDefine.h"

class ANTPlusData;
class ANTPlusPacket
{
public:
	bool mHasRead;					// 是否已经读取过了该数据
	float mLastPacketTime;			// 上一次更新该数据包到当前的时间,单位秒
	ANTPlusPacket* mLastPacket;		// 上一次接收到的包
public:
	unsigned char* mBuffer;			// 存放所有数据的缓冲区
	int mBufferLength;				// 缓冲区大小
	unsigned char mLength;			// 包长度
	unsigned char mCmdID;			// 命令ID
	unsigned char mVersionInfo[2];	// 版本号
	int mHubID;						// hub ID
	ANTPlusData* mData;				// 数据区
	unsigned short mCRC16;			// crc16校验码
	ANTPlusPacket();
	~ANTPlusPacket();
	PARSE_RESULT parseData(unsigned char* data, const int& length);
	void setNewData(ANTPlusPacket* that);
protected:
	void resizeBuffer(const int& size);
	void checkANTPlusData(const unsigned char& cmdID);
	void saveLastPacket();
};

#endif