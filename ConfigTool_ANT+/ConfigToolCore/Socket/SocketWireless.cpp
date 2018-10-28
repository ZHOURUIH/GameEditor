#include "SocketWireless.h"
#include "Utility.h"
#include "WirelessDataManager.h"
#include "StreamBuffer.h"
#include "TimeLock.h"
#include "ANTPlusPacket.h"
#include "ANTPlusPacketHeartBeat.h"
#include "ANTPlusPacketRidingCadence.h"
#include "ANTPlusPacketRidingSpeed.h"
#include "ANTPlusPacketRidingCadenceSpeed.h"
#include "ANTPlusPacketRidingHeartRate.h"

SocketWireless::SocketWireless()
:
mCurElapsedTime(0.0f)
{
	mWirelessStreamBuffer = TRACE_NEW(StreamBuffer, mWirelessStreamBuffer);
	mWirelessStreamBuffer->resizeBuffer(2048);
}

SocketWireless::~SocketWireless()
{
	destroy();
}

void SocketWireless::init(int port)
{
	mPort = port;

	BYTE minorVer = 2;
	BYTE majorVer = 2;
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(minorVer, majorVer);

	int err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		TOOL_CORE_ERROR("WSAStartup err : %d" + WSAGetLastError());
		return;
	}

	if (LOBYTE(wsaData.wVersion) != minorVer ||
		HIBYTE(wsaData.wVersion) != majorVer)
	{
		TOOL_CORE_ERROR("WSAStartup not match ver2.2 err : %d" + WSAGetLastError());
		WSACleanup();
		return;
	}

	mServer = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (mServer == INVALID_SOCKET)
	{
		TOOL_CORE_ERROR("create socket err : %d" + WSAGetLastError());
		return;
	}

	SOCKADDR_IN addrSrv;
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(mPort);
	addrSrv.sin_addr.S_un.S_addr = INADDR_ANY;

	if (bind(mServer, (LPSOCKADDR)&addrSrv, sizeof(addrSrv)) == SOCKET_ERROR)
	{
		TOOL_CORE_ERROR("bind err : %d" + WSAGetLastError());
		return;
	}

	mUDPThread = CreateThread(NULL, 0, updateUdpServer, this, 0, NULL);
	mParseDataThread = CreateThread(NULL, 0, praseDataThread, this, 0, NULL);
}

void SocketWireless::destroy()
{
	CLOSE_THREAD(mUDPThread);
	CLOSE_THREAD(mParseDataThread);
	WSACleanup();
	closesocket(mServer);
}

void SocketWireless::update(float elapsedTime)
{
	LOCK(mReceivePacketBufferLock);
	int count = mReceivePacketBuffer.size();
	for (int i = 0; i < count; ++i)
	{
		mReceivePacketBuffer[i]->execute();
	}
	mReceivePacketBuffer.clear();
	UNLOCK(mReceivePacketBufferLock);
}

DWORD SocketWireless::praseDataThread(LPVOID lpParameter)
{
	SocketWireless* netManager = static_cast<SocketWireless*>(lpParameter);
	TimeLock timeLock(30);
	while (true)
	{
		timeLock.update();
		if (netManager->mWirelessStreamBuffer->mDataLength > 0)
		{
			LOCK(netManager->mReadBufferLock);
			int parsedCount = 0;
			ANT_PLUS_PACKET_TYPE type = ANTPlusPacket::checkType(netManager->mWirelessStreamBuffer->mReadBuffer, netManager->mWirelessStreamBuffer->mDataLength);
			ANTPlusPacket* packet = createPacket(type);
			if (packet != NULL)
			{
				PARSE_RESULT ret = packet->read(netManager->mWirelessStreamBuffer->mReadBuffer, netManager->mWirelessStreamBuffer->mDataLength, parsedCount);
				// 解析成功,则将已解析的数据移除
				if (ret == PR_SUCCESS)
				{
					netManager->mWirelessStreamBuffer->removeDataFromBuffer(0, parsedCount);
					LOCK(netManager->mReceivePacketBufferLock);
					netManager->mReceivePacketBuffer.push_back(packet);
					UNLOCK(netManager->mReceivePacketBufferLock);
				}
				else
				{
					// 解析不成功则释放内存
					TRACE_DELETE(packet);
					// 解析失败,则将缓冲区清空
					if (ret == PR_ERROR)
					{
						netManager->mWirelessStreamBuffer->clearBuffer();
					}
					// 数据不足,继续等待接收数据
					else if (ret == PR_NOT_ENOUGH)
					{
						;
					}
				}
			}
			// 解析失败,则将缓冲区清空
			else
			{
				netManager->mWirelessStreamBuffer->clearBuffer();
			}
			UNLOCK(netManager->mReadBufferLock);
		}
	}
	return 0;
}

DWORD WINAPI SocketWireless::updateUdpServer(LPVOID lpParameter)
{
	SocketWireless* netManager = (SocketWireless*)(lpParameter);
	char buff[1024];
	sockaddr_in addr;
	int nLen = sizeof(addr);
	TimeLock timeLock(5);
	while (true)
	{
		timeLock.update();
		memset(buff, 0, 1024);
		int nRecv = recvfrom(netManager->mServer, buff, 1024, 0, (sockaddr*)&addr, &nLen);
		if (nRecv == 0)
		{
			continue;
		}
		if (nRecv < 0)
		{
			TOOL_CORE_ERROR("网络连接中断! nRecv : %d, error code : %d", nRecv, WSAGetLastError());
			return 0;
		}
		LOCK(netManager->mReadBufferLock);
		netManager->mWirelessStreamBuffer->addDataToBuffer((unsigned char*)buff, nRecv);
		UNLOCK(netManager->mReadBufferLock);
	}
	return 0;
}

ANTPlusPacket* SocketWireless::createPacket(ANT_PLUS_PACKET_TYPE type)
{
	ANTPlusPacket* packet = NULL;
	if (type == APPT_HEART_BEAT)
	{
		packet = TRACE_NEW(ANTPlusPacketHeartBeat, packet, type);
	}
	else if (type == APPT_HEART_RATE)
	{
		packet = TRACE_NEW(ANTPlusPacketRidingHeartRate, packet, type);
	}
	else if (type == APPT_CADENCE)
	{
		packet = TRACE_NEW(ANTPlusPacketRidingCadence, packet, type);
	}
	else if (type == APPT_SPEED)
	{
		packet = TRACE_NEW(ANTPlusPacketRidingSpeed, packet, type);
	}
	else if (type == APPT_CADENCE_SPEED)
	{
		packet = TRACE_NEW(ANTPlusPacketRidingCadenceSpeed, packet, type);
	}
	return packet;
}