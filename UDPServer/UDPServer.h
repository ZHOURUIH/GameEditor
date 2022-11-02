#ifndef _UDP_SERVER_H_
#define _UDP_SERVER_H_

#include "Utility.h"
#include "CommonDefine.h"

class UDPPacket;
class UDPServer : public Utility
{
public:
	UDPServer();
	static UDPServer* get() { return mInstance; }
	void connect(int port);
	void close();
	void update();
	static UDPPacket* createPacket(PACKET_TYPE type);
	void broadcastPacket(UDPPacket* packet, const sockaddr_in& except);
protected:
	void doReceive();
	void doSend(char* data, int count, const sockaddr_in& addr);
protected:
	static UDPServer* mInstance;
	MY_SOCKET mSocket;
	vector<sockaddr_in> mClientList;
};

#endif