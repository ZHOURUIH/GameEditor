#ifndef _SOCKET_NET_MANAGER_H_
#define _SOCKET_NET_MANAGER_H_

#include "CommonDefine.h"

#ifdef _USE_SOCKET_UDP

#include "txMemeryTrace.h"
#include "SocketPacketFactory.h"

struct OUTPUT_STREAM
{
	OUTPUT_STREAM(char* data, int dataSize)
	:
	mData(data),
	mDataSize(dataSize)
	{}
	char* mData;
	int mDataSize;
};

struct INPUT_ELEMENT
{
	INPUT_ELEMENT(SOCKET_PACKET type, char* data, int dataSize)
	:
	mType(type),
	mData(data),
	mDataSize(dataSize)
	{}
	SOCKET_PACKET mType;
	char* mData;
	int mDataSize;
};

class SocketPacketFactoryManager;
// 使用socket时是作为服务器的
class SocketNetManager
{
public:
	SocketNetManager();
	~SocketNetManager();
	void init(const int& port, const int& broadcastPort);
	void update(const float& elapsedTime);
	void destroy();
	void heartBeat(){}
	void processOutput(){}
	void processInput();
	SocketPacket* createPacket(const SOCKET_PACKET& type);
	void destroyPacket(SocketPacket* packet);
	void sendMessage(SocketPacket* packet, const bool& destroyPacketEndSend = true);
protected:
	static DWORD WINAPI updateUdpServer(LPVOID lpParameter);
	static DWORD WINAPI updateOutput(LPVOID lpParameter);
	void receivePacket(const SOCKET_PACKET& type, char* data, const int& dataSize)
	{
		char* p = TRACE_NEW_ARRAY(char, dataSize, p);
		memcpy(p, data, dataSize);
		INPUT_ELEMENT element(type, p, dataSize);
		// 等待解锁接收流的读写,并锁定接收流
		WaitForSingleObject(mInputMutex, INFINITE);
		mReceiverStream.push_back(element);
		// 锁定接收流的读写
		ReleaseMutex(mInputMutex);
	}
	static SOCKET_PACKET checkPacketType(char* buffer, int bufferLen);
protected:
	SocketPacketFactoryManager*	mSocketPacketFactoryManager;
	txVector<OUTPUT_STREAM>		mOutputStreamList;		// 输出流
	float						mCurrElapsedTime;		// 当前计时 
	int							mPort;                  // 端口号
	int							mBroadcastPort;			// 广播的端口号
	SOCKET						mServer;				// 用于接收消息的套接字
	SOCKET						mBroadcastSocket;		// 用于广播的套接字
	SOCKADDR_IN					mBroadcastAddr;			// 广播地址
	txVector<INPUT_ELEMENT>		mInputStreamList;		// 输入流
	HANDLE						mInputMutex;			// 锁定读写输入流,锁定后不能读写输入流
	HANDLE						mSocketThread;			// 套接字线程句柄
	HANDLE						mOutputMutex;			// 输出流互斥锁
	HANDLE						mOutputThread;			// 输出流发送线程
	txVector<INPUT_ELEMENT>		mReceiverStream;		// 接收的流
};

#endif

#endif