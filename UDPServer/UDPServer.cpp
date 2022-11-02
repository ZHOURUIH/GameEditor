#include "UDPServer.h"
#include "UDPPacket.h"
#include "CSPacketInit.h"
#include "CSPacketMove.h"
#include "SCPacketOtherMove.h"

UDPServer* UDPServer::mInstance = nullptr;

UDPServer::UDPServer()
{
	mInstance = this;
}

void UDPServer::connect(int port)
{
#ifdef WIN32
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		ERROR("WSAStartup failed!");
		return;
	}
#endif
	//创建监听的Socket
	mSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (mSocket == INVALID_SOCKET)
	{
		ERROR("socket failed!");
		return;
	}

	//设置服务器Socket地址
	sockaddr_in addrServ;
	makeSockAddr(addrServ, INADDR_ANY, port);
	//绑定Sockets Server
	if (bind(mSocket, (const struct sockaddr*)&addrServ, sizeof(sockaddr_in)) != 0)
	{
		mSocket = INVALID_SOCKET;
		ERROR("bind failed!");
		return;
	}
#ifdef WIN32
	int timeout = 2000;
	int ret = setsockopt(mSocket, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));
	ret |= setsockopt(mSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
#else
	struct timeval timeout = { 2, 0 };
	const int ret = setsockopt(mSocket, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(struct timeval));
#endif
	if (ret != 0)
	{
		ERROR("设置socket选项失败");
		return;
	}
}

void UDPServer::close()
{
	if (mSocket != INVALID_SOCKET)
	{
		CLOSE_SOCKET(mSocket);
		mSocket = INVALID_SOCKET;
	}
}

void UDPServer::update()
{
	doReceive();
}

void UDPServer::doReceive()
{
	constexpr int RecvBufferSize = 2048;
	thread_local static char RecvBuffer[RecvBufferSize];
	sockaddr_in fromAddr;
#ifdef WIN32
	int fromLen = sizeof(fromAddr);
#else
	socklen_t fromLen = sizeof(fromAddr);
#endif
	const int nRecv = recvfrom(mSocket, RecvBuffer, RecvBufferSize, 0, (sockaddr*)&fromAddr, &fromLen);
	bool hasThisClient = false;
	for (const auto& item : mClientList)
	{
		if (item.sin_port == fromAddr.sin_port)
		{
			hasThisClient = true;
			break;
		}
	}
	if (!hasThisClient)
	{
		mClientList.push_back(fromAddr);
	}

	// 接收到一个数据以后,向其他客户端广播此数据
	int readIndex = 0;
	UDPPacket* packet = createPacket((PACKET_TYPE)RecvBuffer[0]);
	packet->read(RecvBuffer, RecvBufferSize, readIndex);
	if (readIndex > nRecv)
	{
		ERROR("接收数据错误");
	}
	packet->execute(fromAddr);
	delete packet;
}

void UDPServer::doSend(char* data, int count, const sockaddr_in& addr)
{
	sendto(mSocket, data, count, 0, (sockaddr*)&addr, sizeof(addr));
}

UDPPacket* UDPServer::createPacket(const PACKET_TYPE type)
{
	UDPPacket* packet = nullptr;
	switch (type)
	{
	case PACKET_TYPE::CS_INIT: packet = new CSPacketInit(); break;
	case PACKET_TYPE::CS_MOVE: packet = new CSPacketMove(); break;
	case PACKET_TYPE::SC_OTHER_MOVE: packet = new CSPacketMove(); break;
	default:break;
	}
	if (packet != nullptr)
	{
		packet->setType(type);
	}
	return packet;
}

void UDPServer::broadcastPacket(UDPPacket* packet, const sockaddr_in& except)
{
	char sendBuffer[2048]{0};
	int packetSize = 0;
	packet->write(sendBuffer, 2048, packetSize);
	for (const auto& item : mClientList)
	{
		if (item.sin_port == except.sin_port)
		{
			continue;
		}
		doSend(sendBuffer, packetSize, item);
	}
}