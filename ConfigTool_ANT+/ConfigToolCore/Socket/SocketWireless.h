#ifndef _SOCKET_WIRELESS_H_
#define _SOCKET_WIRELESS_H_

#include "CommonDefine.h"
#include "ThreadLock.h"
#include "ToolCoreBase.h"

class WriteData
{
public:
	char* mData;
	int mDataCount;
};

// 无线设备的UDP网络管理器
class StreamBuffer;
class ANTPlusPacket;
class SocketWireless : public ToolCoreBase
{
public:
	SocketWireless();
	~SocketWireless();
	void init(int port);
	void update(float elapsedTime);
	void destroy();
	static ANTPlusPacket* createPacket(ANT_PLUS_PACKET_TYPE type);
protected:
	static DWORD WINAPI updateUdpServer(LPVOID lpParameter);
	static DWORD WINAPI updateOutput(LPVOID lpParameter);
	static DWORD WINAPI praseDataThread(LPVOID lpParameter);
protected:
	float					mCurElapsedTime;		// 当前计时 
	int						mPort;                  // 端口号
	SOCKET					mServer;				// 用于接收消息的套接字
	HANDLE					mUDPThread;				// 套接字线程句柄
	HANDLE					mParseDataThread;		// 解析数据的线程
	ThreadLock				mReadBufferLock;
	std::vector<WriteData*>	mWriteDataBuffer;		// 写入数据的缓冲列表,用于添加数据
	std::vector<WriteData*>	mWriteDataList;			// 需要写入的数据列表,用于写入数据
	StreamBuffer*			mWirelessStreamBuffer;
	txVector<ANTPlusPacket*>mReceivePacketBuffer;
	ThreadLock				mReceivePacketBufferLock;
};

#endif