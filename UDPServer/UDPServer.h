#ifndef _UDP_SERVER_H_
#define _UDP_SERVER_H_

#include "CommandReceiver.h"
#include "CommonDefine.h"
#include "Map.h"

class UDPServerClient;
class UDPPacket;
class CustomThread;
class UDPServer : public CommandReceiver
{
public:
	UDPServer();
	static UDPServer* get() { return mInstance; }
	void connect(int port);
	void close();
	void update();
	const Map<llong, sockaddr_in>& getClientList() const { return mClientList; }
	void addClient(const sockaddr_in& addr) { mClientList.tryInsert(addr.sin_port) = addr; }
	static UDPPacket* createPacket(PACKET_TYPE type);
	void broadcastPacket(UDPPacket* packet, llong except);
protected:
	void doSend(char* data, int count, const sockaddr_in& addr);
	static void sendSocket(CustomThread* thread);
	static void receiveSocket(CustomThread* thread);
protected:
	static UDPServer* mInstance;
	MY_SOCKET mSocket = 0;
	Map<llong, sockaddr_in> mClientList;
	CustomThread* mSendThread = nullptr;			// 发送线程
	CustomThread* mReceiveThread = nullptr;			// 接收线程
	UDPServerClient* mClient = nullptr;
	set<llong> mClientTokenList;
	bool mUseMultiThread = false;
};

#endif