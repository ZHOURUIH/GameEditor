#include "UDPServerClient.h"
#include "UDPPacket.h"
#include "UDPClientInfo.h"
#include "UDPServer.h"
#include "CmdNetServerReceiveUDPPacket.h"
#include "CommandSystem.h"

static constexpr int UDP_MAX_PACKET_SIZE = 60 * 1024;			// udp单个消息包最大的大小,60KB

UDPServerClient::UDPServerClient()
{}

void UDPServerClient::destroy()
{}

void UDPServerClient::boradcast(UDPPacket* packet, const llong exceptToken)
{
	const auto& clientList = UDPServer::get()->getClientList();
	for(const auto& iter : clientList.mMap)
	{
		if (iter.first == exceptToken)
		{
			continue;
		}
		sendPacket(packet, iter.first, iter.second);
	}
}

void UDPServerClient::sendPacket(UDPPacket* packet, const llong token, const sockaddr_in& addr)
{
	// 该函数可能在多线程中调用,所以需要添加线程锁防止重入
	LOCK(mSendPacketLock);
	FOR_ONCE
	{
		if (addr.sin_port == 0)
		{
			break;
		}
		char* pBuffer = new char[1024];
		int packetSize = 0;
		packet->write(pBuffer, 1024, packetSize);

		// 放入到缓存列表中
		auto& writeBuffer = mSendList.startWrite();
		UDPClientInfo* info = nullptr;
		const auto* clientInfoPtr = writeBuffer.get(token);
		if (clientInfoPtr == nullptr)
		{
			info = new UDPClientInfo();
			info->setToken(token);
			writeBuffer.insert(token, info);
		}
		else
		{
			info = *clientInfoPtr;
		}
		// 虽然一般来说一个客户端的地址不会改变,但是为了保证一定正确,每次发送时都要设置一次地址
		info->setAddress(addr);
		
		info->addSendData(pBuffer, packetSize);
		mSendList.endWrite();
	}
	UNLOCK(mSendPacketLock);
}

void UDPServerClient::removeClientToken(const llong token)
{
	auto& writeBuffer = mSendList.startWrite();
	UDPClientInfo* info = nullptr;
	writeBuffer.erase(token, info);
	delete info;
	mSendList.endWrite();
}

void UDPServerClient::recvData(char* data, const int dataCount, const int bufferSize, const sockaddr_in& addr)
{
	int index = 0;
	UDPPacket* packet = nullptr;
	llong token = 0;
	// udp基于包的传输,解析出现错误或者未接收完全时都不会再解析
	if (!packetRead(data, dataCount, bufferSize, index, packet, token))
	{
		return;
	}
	CmdNetServerReceiveUDPPacket* cmd = new CmdNetServerReceiveUDPPacket();
	cmd->mAddress = addr;
	cmd->mPacketList.push_back(packet);
	CommandSystem::get()->pushCommandDelay(cmd, UDPServer::get());
}

bool UDPServerClient::packetRead(char* buffer, const int dataLength, const int bufferSize, int& index, UDPPacket*& packet, llong& token)
{
	// 可能还没有接收完全,等待下次接收
	if (dataLength < index + 1)
	{
		return false;
	}
	int readIndex = 0;
	packet = UDPServer::createPacket((PACKET_TYPE)buffer[0]);
	packet->read(buffer, bufferSize, readIndex);
	index += readIndex;
	return true;
}