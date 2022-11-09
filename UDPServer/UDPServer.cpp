#include "UDPServer.h"
#include "UDPPacket.h"
#include "CSPacketInit.h"
#include "CSPacketMove.h"
#include "SCPacketOtherMove.h"
#include "ThreadManager.h"
#include "CustomThread.h"
#include "UDPServerClient.h"
#include "UDPClientInfo.h"

UDPServer* UDPServer::mInstance = nullptr;

UDPServer::UDPServer()
{
	mInstance = this;
	mClient = new UDPServerClient();
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
#else
	struct timeval timeout = { 2, 0 };
	const int ret = setsockopt(mSocket, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(struct timeval));
#endif
	if (ret != 0)
	{
		ERROR("设置socket选项失败");
		return;
	}
	cout << "启动服务器:IP:" << getLocalIP(mSocket) << ",端口" << port << endl;
	if (mUseMultiThread)
	{
		mSendThread = ThreadManager::get()->createThread("SendSocketUDP", sendSocket, this);
		mReceiveThread = ThreadManager::get()->createThread("ReceiveSocketUDP", receiveSocket, this);
		mSendThread->setTime(10);
		mReceiveThread->setTime(0, 0);
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

void UDPServer::sendSocket(CustomThread* thread)
{
	const auto* netManager = static_cast<UDPServer*>(thread->getArgs());
	// 发送消息
	try
	{
		thread_local static int mIndex = 0;
		auto* sendListPtr = netManager->mClient->startReadSendList();
		if (sendListPtr != nullptr)
		{
			auto& sendList = *sendListPtr;
			for(auto iter : sendList)
			{
				UDPClientInfo* clientInfo = iter.second;
				const sockaddr_in& addr = clientInfo->getAddress();
				auto& curSendList = clientInfo->getSendList();
				for (int i = 0; i < curSendList.size(); ++i)
				{
					LOG("发送消息,长度:" + intToString(curSendList[i].second) + ", 下标:" + intToString(mIndex++) + ",目标端口:" + intToString(addr.sin_port));
					sendto(netManager->mSocket, curSendList[i].first, curSendList[i].second, 0, (sockaddr*)&addr, sizeof(addr));
				}
				clientInfo->clear();
			}
		}
		netManager->mClient->endReadSendList();
	}
	catch (const exception& e)
	{
		LOG("exception : " + string(e.what()));
	}
}

void UDPServer::receiveSocket(CustomThread* thread)
{
	const auto* netManager = static_cast<UDPServer*>(thread->getArgs());
	// 接收消息
	try
	{
		sockaddr_in fromAddr;
#ifdef WIN32
		int fromLen = sizeof(fromAddr);
#else
		socklen_t fromLen = sizeof(fromAddr);
#endif
		constexpr int RecvBufferSize = 2048;
		thread_local static char RecvBuffer[RecvBufferSize];
		thread_local static int mIndex;
		const int nRecv = recvfrom(netManager->mSocket, RecvBuffer, RecvBufferSize, 0, (sockaddr*)&fromAddr, &fromLen);
		if (nRecv > 0)
		{
			LOG("收到消息,长度:" + intToString(nRecv) + ", 下标:" + intToString(mIndex++) + ",接收端口:" + intToString(fromAddr.sin_port) + ", 时间:" + getTime());
			netManager->mClient->recvData(RecvBuffer, nRecv, RecvBufferSize, fromAddr);
		}
	}
	catch (const exception& e)
	{
		LOG("exception : " + string(e.what()));
	}
}

void UDPServer::update()
{
	if (!mUseMultiThread)
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
		if (nRecv < 0)
		{
			cout << "错误码:" << errno << endl;
		}
		else if (nRecv > 0)
		{
			int readIndex = 0;
			UDPPacket* packet = createPacket((PACKET_TYPE)RecvBuffer[0]);
			packet->read(RecvBuffer, RecvBufferSize, readIndex);
			static llong lastTime = timeGetTime();
			LOG("收到消息,长度:" + intToString(nRecv) + ",接收端口:" + intToString(fromAddr.sin_port) + ", 时间:" + getTime() + ", 时间差:" + intToString(timeGetTime() - lastTime));
			lastTime = timeGetTime();
			bool hasThisClient = false;
			for (const auto& item : mClientList)
			{
				if (item.second.sin_port == fromAddr.sin_port)
				{
					hasThisClient = true;
					break;
				}
			}
			if (!hasThisClient)
			{
				mClientList.insert(fromAddr.sin_port, fromAddr);
			}

			// 接收到一个数据以后,向其他客户端广播此数据

			if (readIndex > nRecv)
			{
				ERROR("接收数据错误");
			}
			packet->execute(fromAddr.sin_port);
			delete packet;
		}
	}
}

void UDPServer::doSend(char* data, int count, const sockaddr_in& addr)
{
	LOG("发送消息,长度:" + intToString(count) + ",目标端口:" + intToString(addr.sin_port));
	sendto(mSocket, data, count, 0, (sockaddr*)&addr, sizeof(addr));
}

UDPPacket* UDPServer::createPacket(const PACKET_TYPE type)
{
	UDPPacket* packet = nullptr;
	switch (type)
	{
	case PACKET_TYPE::CS_INIT: packet = new CSPacketInit(); break;
	case PACKET_TYPE::CS_MOVE: packet = new CSPacketMove(); break;
	case PACKET_TYPE::SC_OTHER_MOVE: packet = new SCPacketOtherMove(); break;
	default:break;
	}
	if (packet != nullptr)
	{
		packet->setType(type);
	}
	return packet;
}

void UDPServer::broadcastPacket(UDPPacket* packet, llong except)
{
	if (mUseMultiThread)
	{
		mClient->boradcast(packet, except);
	}
	else
	{
		char sendBuffer[2048]{ 0 };
		int packetSize = 0;
		packet->write(sendBuffer, 2048, packetSize);
		for (const auto& item : mClientList)
		{
			if (item.first == except)
			{
				continue;
			}
			doSend(sendBuffer, packetSize, item.second);
		}
	}
}