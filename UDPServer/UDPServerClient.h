#ifndef _UDP_SERVER_CLIENT_H_
#define _UDP_SERVER_CLIENT_H_

#include "Map.h"
#include "ThreadLock.h"
#include "DoubleBufferNoListThread.h"

// 当前程序是UDP服务器时,处理所有客户端的逻辑
class UDPClientInfo;
class UDPPacket;
class UDPServerClient
{
public:
	UDPServerClient();
	virtual ~UDPServerClient() { destroy(); }
	void destroy();
	void boradcast(UDPPacket* packet, const llong exceptToken);
	void sendPacket(UDPPacket* packet, const llong token, const sockaddr_in& addr);
	static void recvData(char* data, const int dataCount, const int bufferSize, const sockaddr_in& addr);
	Map<llong, UDPClientInfo*>* startReadSendList() { return mSendList.startRead(); }
	void endReadSendList() { return mSendList.endRead(); }
	void removeClientToken(const llong token);
protected:
	static bool packetRead(char* buffer, const int dataLength, const int bufferSize, int& index, UDPPacket*& packet, llong& token);
protected:
	DoubleBufferNoListThread<Map<llong, UDPClientInfo*>> mSendList;	// 所有地址的待发送消息列表
	ThreadLock mSendPacketLock;										// 防止sendPacket函数多线程调用时重入的线程锁
};

#endif