#include "SocketNetManager.h"
#include "ToolCoreLog.h"
#include "ToolCoreUtility.h"
#include "DataManager.h"
#include "txMemoryTrace.h"

#define CLOSE_THREAD(thread)	\
if (thread != NULL)				\
{								\
	TerminateThread(thread, 0);	\
	thread = NULL;				\
}

SocketNetManager::SocketNetManager()
:
mCurElapsedTime(0.0f)
{
	resizeBuffer(2048);
}

SocketNetManager::~SocketNetManager()
{
	destroy();
}

void SocketNetManager::init(int port)
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

void SocketNetManager::destroy()
{
	CLOSE_THREAD(mUDPThread);
	CLOSE_THREAD(mParseDataThread);
	WSACleanup();
	closesocket(mServer);
}

DWORD SocketNetManager::praseDataThread(LPVOID lpParameter)
{
	SocketNetManager* netManager = static_cast<SocketNetManager*>(lpParameter);
	while (true)
	{
		Sleep(30);
		if (netManager->mDataLength > 0)
		{
			LOCK(netManager->mReadBufferLock);
			int parsedCount = 0;
			PARSE_RESULT ret = mDataManager->setData(netManager->mReadBuffer, netManager->mDataLength, parsedCount);
			// 解析成功,则将已解析的数据移除
			if (ret == PR_SUCCESS)
			{
				netManager->removeDataFromBuffer(0, parsedCount);
			}
			// 解析失败,则将缓冲区清空
			else if (ret == PR_ERROR)
			{
				netManager->clearBuffer();
			}
			// 数据不足,继续等待接收数据
			else if (ret == PR_NOT_ENOUGH)
			{
				;
			}
			UNLOCK(netManager->mReadBufferLock);
		}
	}
	return 0;
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
			TOOL_CORE_ERROR("网络连接中断! nRecv : %d, error code : %d", nRecv, WSAGetLastError());
			return 0;
		}
		LOCK(netManager->mReadBufferLock);
		netManager->addDataToBuffer((unsigned char*)buff, nRecv);
		std::string str = StringUtility::charArrayToHexString((unsigned char*)buff, nRecv) + "\n";
		OutputDebugStringA(str.c_str());
		UNLOCK(netManager->mReadBufferLock);
	}
	return 0;
}

void SocketNetManager::resizeBuffer(const int& size)
{
	if (mBufferSize >= size)
	{
		return;
	}
	LOCK(mReadBufferLock);
	if (mReadBuffer != NULL)
	{
		// 创建新的缓冲区,将原来的数据拷贝到新缓冲区中,销毁原缓冲区,指向新缓冲区
		unsigned char* newBuffer = TRACE_NEW_ARRAY(unsigned char, size, newBuffer);
		if (mDataLength > 0)
		{
			memcpy(newBuffer, mReadBuffer, mDataLength);
		}
		TRACE_DELETE_ARRAY(mReadBuffer);
		mReadBuffer = newBuffer;
		mBufferSize = size;
	}
	else
	{
		mReadBuffer = TRACE_NEW_ARRAY(unsigned char, size, mReadBuffer);
		mBufferSize = size;
	}
	UNLOCK(mReadBufferLock);
}

void SocketNetManager::addDataToBuffer(unsigned char* data, const int& dataCount)
{
	// 如果当前已经存放不下新的数据了,不再处理新数据
	if (mBufferSize - mDataLength < dataCount)
	{
		return;
	}
	memcpy(mReadBuffer + mDataLength, data, dataCount);
	mDataLength += dataCount;
}

void SocketNetManager::removeDataFromBuffer(const int& start, const int& count)
{
	if (mDataLength < start + count)
	{
		return;
	}
	memmove(mReadBuffer + start, mReadBuffer + start + count, mDataLength - start - count);
	mDataLength -= count;
}

void SocketNetManager::clearBuffer()
{
	mDataLength = 0;
}