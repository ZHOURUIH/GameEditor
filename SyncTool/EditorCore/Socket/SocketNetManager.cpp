#include "SocketNetManager.h"

#ifdef _USE_SOCKET_UDP

#include "EditorCoreLog.h"
#include "SocketPacketFactory.h"
#include "SocketPacketFactoryManager.h"

SocketNetManager::SocketNetManager()
:
mCurrElapsedTime(0.0f)
{
	mInputMutex = CreateMutex(NULL, false, NULL);
	mOutputMutex = CreateMutex(NULL, false, NULL);
}

SocketNetManager::~SocketNetManager()
{
	destroy();
}

void SocketNetManager::init(const int& port, const int& broadcastPort)
{
	mPort = port;
	mBroadcastPort = broadcastPort;

	// 初始化工厂
	mSocketPacketFactoryManager = TRACE_NEW(SocketPacketFactoryManager, mSocketPacketFactoryManager);
	mSocketPacketFactoryManager->init();

	BYTE minorVer = 2;
	BYTE majorVer = 2;
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(minorVer, majorVer);

	int err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		EDITOR_CORE_ERROR("WSAStartup err : %d" + WSAGetLastError());
		return;
	}

	if (LOBYTE(wsaData.wVersion) != minorVer ||
		HIBYTE(wsaData.wVersion) != majorVer)
	{
		EDITOR_CORE_ERROR("WSAStartup not match ver2.2 err : %d" + WSAGetLastError());
		WSACleanup();
		return;
	}

	mServer = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (mServer == INVALID_SOCKET)
	{
		EDITOR_CORE_ERROR("create socket err : %d" + WSAGetLastError());
		return;
	}

	mBroadcastSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (mBroadcastSocket == INVALID_SOCKET)
	{
		EDITOR_CORE_ERROR("create broadcast socket err : %d" + WSAGetLastError());
		return;
	}

	mBroadcastAddr.sin_family = AF_INET;
	mBroadcastAddr.sin_port = htons(mBroadcastPort);
	mBroadcastAddr.sin_addr.s_addr = INADDR_BROADCAST;

	bool bOpt = true;
	//设置该套接字为广播类型
	setsockopt(mBroadcastSocket, SOL_SOCKET, SO_BROADCAST, (char*)&bOpt, sizeof(bOpt));

	SOCKADDR_IN addrSrv;
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(mPort);
	addrSrv.sin_addr.S_un.S_addr = INADDR_ANY;

	if (bind(mServer, (LPSOCKADDR)&addrSrv, sizeof(addrSrv)) == SOCKET_ERROR)
	{
		EDITOR_CORE_ERROR("bind err : %d" + WSAGetLastError());
		return;
	}

	mSocketThread = CreateThread(NULL, 0, SocketNetManager::updateUdpServer, this, 0, NULL);
	mOutputThread = CreateThread(NULL, 0, SocketNetManager::updateOutput, this, 0, NULL);
}

void SocketNetManager::update(const float& elapsedTime)
{
	mCurrElapsedTime += elapsedTime;
	processOutput();
	processInput();
	if (mCurrElapsedTime >= 60.0f)
	{
		heartBeat();
		mCurrElapsedTime = 0.0f;
	}
}

void SocketNetManager::destroy()
{
	TerminateThread(mSocketThread, 0);
	TerminateThread(mOutputThread, 0);
	WSACleanup();
	closesocket(mServer);
	closesocket(mBroadcastSocket);
	CloseHandle(mInputMutex);
	CloseHandle(mOutputMutex);
	TRACE_DELETE(mSocketPacketFactoryManager);
}

void SocketNetManager::processInput()
{
	// 等待解锁接收流的读写,并锁定接收流
	WaitForSingleObject(mInputMutex, INFINITE);
	int receiveCount = mReceiverStream.size();
	for (int i = 0; i < receiveCount; ++i)
	{
		mInputStreamList.push_back(mReceiverStream[i]);
	}
	mReceiverStream.clear();
	// 锁定接收流的读写
	ReleaseMutex(mInputMutex);

	int streamCount = mInputStreamList.size();
	for (int i = 0; i < streamCount; ++i)
	{
		INPUT_ELEMENT& element = mInputStreamList[i];
		SocketPacket* packetReply = createPacket(element.mType);
		if (packetReply != NULL)
		{
			packetReply->read(element.mData, element.mDataSize);
			packetReply->execute();
			destroyPacket(packetReply);
		}
		TRACE_DELETE_ARRAY(element.mData);
	}
	mInputStreamList.clear();
}

SocketPacket* SocketNetManager::createPacket(const SOCKET_PACKET& type)
{
	if (type == SP_MAX)
	{
		return NULL;
	}
	return mSocketPacketFactoryManager->getFactory(type)->createPacket();
}

void SocketNetManager::destroyPacket(SocketPacket* packet)
{
	if (packet == NULL)
	{
		return;
	}
	mSocketPacketFactoryManager->getFactory(packet->getPacketType())->destroyPacket(packet);
}

void SocketNetManager::sendMessage(SocketPacket* packet, const bool& destroyPacketEndSend)
{
	// 将包的数据存入列表
	int packetSize = packet->getSize();
	char* packetData = TRACE_NEW_ARRAY(char, packetSize, packetData);
	packet->write(packetData, packetSize);

	// 保存发送数据
	WaitForSingleObject(mOutputMutex, INFINITE);
	mOutputStreamList.push_back(OUTPUT_STREAM(packetData, packetSize));
	ReleaseMutex(mOutputMutex);

	if (destroyPacketEndSend)
	{
		destroyPacket(packet);
	}
}

DWORD WINAPI SocketNetManager::updateUdpServer(LPVOID lpParameter)
{
	SocketNetManager* netManager = (SocketNetManager*)(lpParameter);
	char buff[1024];
	sockaddr_in addr;
	int nLen = sizeof(addr);
	while (true)
	{
		memset(buff, 0, 1024);
		int nRecv = recvfrom(netManager->mServer, buff, 1024, 0, (sockaddr*)&addr, &nLen);
		if (nRecv == 0)
		{
			continue;
		}
		if (nRecv < 0)
		{
			EDITOR_CORE_ERROR("网络连接中断! nRecv : %d, error code : %d", nRecv, WSAGetLastError());
			return 0;
		}
		// 判断消息包类型
		SOCKET_PACKET type = checkPacketType(buff, nRecv);
		netManager->receivePacket(type, buff, nRecv);
	}
	return 0;
}

DWORD WINAPI SocketNetManager::updateOutput(LPVOID lpParameter)
{
	SocketNetManager* netManager = (SocketNetManager*)(lpParameter);
	SOCKADDR* broadcastAddr = (SOCKADDR*)&netManager->mBroadcastAddr;
	int broadcastAddrLen = sizeof(netManager->mBroadcastAddr);
	while (true)
	{
		// 如果有需要发送的数据,则先发送数据
		WaitForSingleObject(netManager->mOutputMutex, INFINITE);
		int outputCount = netManager->mOutputStreamList.size();
		if (outputCount > 0)
		{
			for (int i = 0; i < outputCount; ++i)
			{
				OUTPUT_STREAM& outStream = netManager->mOutputStreamList[i];
				int nSendSize = sendto(netManager->mBroadcastSocket, outStream.mData, outStream.mDataSize, 0, broadcastAddr, broadcastAddrLen);
				TRACE_DELETE_ARRAY(outStream.mData);
				if (SOCKET_ERROR == nSendSize)
				{
					EDITOR_CORE_ERROR("广播错误， error code : %d", WSAGetLastError());
					ReleaseMutex(netManager->mOutputMutex);
					return 0;
				}
			}
			netManager->mOutputStreamList.clear();
		}
		ReleaseMutex(netManager->mOutputMutex);
	}
}

SOCKET_PACKET SocketNetManager::checkPacketType(char* buffer, int bufferLen)
{
	if (bufferLen < 2)
	{
		return SP_MAX;
	}
	return SP_MAX;
}

#endif