#ifndef _SOCKET_NET_MANAGER_H_
#define _SOCKET_NET_MANAGER_H_

#include "CommonDefine.h"
#include "txThreadLock.h"
#include "ToolCoreBase.h"

class WriteData
{
public:
	char* mData;
	int mDataCount;
};

class SocketNetManager : public ToolCoreBase
{
public:
	SocketNetManager();
	~SocketNetManager();
	void init(int port);
	void update(float elapsedTime){}
	void destroy();
protected:
	static DWORD WINAPI updateUdpServer(LPVOID lpParameter);
	static DWORD WINAPI updateOutput(LPVOID lpParameter);
	static DWORD WINAPI praseDataThread(LPVOID lpParameter);
	void resizeBuffer(const int& size);
	void addDataToBuffer(unsigned char* data, const int& dataCount);
	void removeDataFromBuffer(const int& start, const int& count);
	void clearBuffer();
protected:
	float					mCurElapsedTime;		// 当前计时 
	int						mPort;                  // 端口号
	SOCKET					mServer;				// 用于接收消息的套接字
	HANDLE					mUDPThread;				// 套接字线程句柄
	HANDLE					mParseDataThread;		// 解析数据的线程
	txThreadLock			mReadBufferLock;
	unsigned char*			mReadBuffer;			// 接收到的数据的缓冲区
	std::atomic<int>		mDataLength;			// 缓冲区中剩余接收到的数据长度
	std::atomic<int>		mBufferSize;			// 缓冲区总大小
	std::vector<WriteData*>	mWriteDataBuffer;		// 写入数据的缓冲列表,用于添加数据
	std::vector<WriteData*>	mWriteDataList;			// 需要写入的数据列表,用于写入数据
};

#endif