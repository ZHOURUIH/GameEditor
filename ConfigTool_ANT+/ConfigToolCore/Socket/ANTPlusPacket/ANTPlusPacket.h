#ifndef _ANT_PLUS_PACKET_H_
#define _ANT_PLUS_PACKET_H_

#include "CommonDefine.h"
#include "ToolCoreBase.h"

class ANTPlusData;
class ANTPlusPacket : public ToolCoreBase
{
public:
	float mLastReceivePacketTime;	// 上一次接收到该数据包到当前的时间,单位秒.
	float mLastUpdatePacketTime;	// 上一次更新该数据包到当前的时间,单位秒,与接收的时间不同,接收到数据时不一定会更新数据
	ANTPlusPacket* mLastPacket;		// 上一次接收到的包
	ANT_PLUS_PACKET_TYPE mType;		// 包的类型
public:
	unsigned char* mBuffer;			// 存放所有数据的缓冲区
	int mBufferLength;				// 缓冲区大小
	unsigned char mLength;			// 包长度
	unsigned char mCmdID;			// 命令ID
	unsigned char mVersionInfo[2];	// 版本号
	int mHubID;						// hub ID
	ANTPlusData* mData;				// 数据区
	unsigned short mCRC16;			// crc16校验码
public:
	ANTPlusPacket(ANT_PLUS_PACKET_TYPE type);
	~ANTPlusPacket();
	void update(float elapsedTime);
	PARSE_RESULT read(unsigned char* data, int length, int& readCount);
	void execute();
	void updateData(ANTPlusPacket* lastPacket);
	void notifyReceived();
	static PARSE_RESULT parse(ANTPlusPacket* packet, unsigned char* data, int length);
	static void copyPacket(ANTPlusPacket* source, ANTPlusPacket* dest);
	static ANT_PLUS_PACKET_TYPE checkType(unsigned char* data, int length);
protected:
	void resizeBuffer(int size);
	virtual void checkANTPlusData() = 0;
	virtual void writeReceiveLog(){}
	virtual void writeUpdateLog(){}
	void resetLastReceiveTime() { mLastReceivePacketTime = 0.0f; }
	void resetLastUpdateTime() { mLastUpdatePacketTime = 0.0f; }
};

#endif