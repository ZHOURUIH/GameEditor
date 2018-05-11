#include "txEngineDefine.h"

#if RUN_PLATFORM == PLATFORM_ANDROID
#include <errno.h>
#include <time.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#elif RUN_PLATFORM == PLATFORM_WINDOWS
#endif

#include "txNetSystem.h"
#include "txNetNotify.h"
#include "txEngineLog.h"
#include "Utility.h"
#include "txEngineRoot.h"

txNetSystem::txNetSystem()
{
	;
}

txNetSystem::~txNetSystem()
{
	deInit();
}

int txNetSystem::init(txNetNotify *pNotify, const bool& showError)
{
#if RUN_PLATFORM == PLATFORM_WINDOWS
	BYTE minorVer = 2;
	BYTE majorVer = 2;
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(minorVer, majorVer);

	int err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		if (showError)
		{
			ENGINE_ERROR("WSAStartup err : %d" + WSAGetLastError());
		}
		else
		{
			LOGI("WSAStartup err : %d" + WSAGetLastError());
		}
		return -1;
	}

	if (LOBYTE(wsaData.wVersion) != minorVer ||
		HIBYTE(wsaData.wVersion) != majorVer)
	{
		if (showError)
		{
			ENGINE_ERROR("WSAStartup not match ver2.2 err : %d" + WSAGetLastError());
		}
		else
		{
			LOGI("WSAStartup not match ver2.2 err : %d" + WSAGetLastError());
		}
		WSACleanup();
		return -1;
	}
#endif
	m_pNotify = pNotify;
	m_SockID = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_SockID == INVALID_SOCKET)
	{
		m_sockErr = errno;
		if (showError)
		{
			ENGINE_ERROR("error : can not create socket! failed err(%d)[%s].", errno, getErrInfo());
		}
		else
		{
			LOGI("error : can not create socket! failed err(%d)[%s].", errno, getErrInfo());
		}
		return -1;
	}

	memset(&m_sockAddr, 0, sizeof(sockaddr_in));
	m_sockAddr.sin_family = AF_INET;

	// send buffer size
	int size = 2048;
	int nRet = setsockopt(m_SockID, SOL_SOCKET, SO_SNDBUF, (const char*) &size, sizeof(int));
	if (SOCKET_ERROR == nRet)
	{
		m_sockErr = errno;
		if (showError)
		{
			ENGINE_ERROR("error : can not set send scoket opt! failed err(%d)[%s].", errno, getErrInfo());
		}
		else
		{
			LOGI("error : can not set send scoket opt! failed err(%d)[%s].", errno, getErrInfo());
		}
		return -1;
	}

	// recv buffer size
	nRet = setsockopt(m_SockID, SOL_SOCKET, SO_RCVBUF, (const char*) &size, sizeof(int));
	if (SOCKET_ERROR == nRet)
	{
		m_sockErr = errno;
		if (showError)
		{
			ENGINE_ERROR("error : can not set recv scoket opt! failed err(%d)[%s].", errno, getErrInfo());
		}
		else
		{
			LOGI("error : can not set recv scoket opt! failed err(%d)[%s].", errno, getErrInfo());
		}
		return -1;
	}

	// linger
	int lingerTime = 2000;
	struct linger lin;
	lin.l_onoff = lingerTime > 0 ? 1 : 0;
	lin.l_linger = lingerTime;
	nRet = setsockopt(m_SockID, SOL_SOCKET, SO_LINGER, (const char*) &lin, sizeof(lin));
	if (SOCKET_ERROR == nRet)
	{
		m_sockErr = errno;
		if (showError)
		{
			ENGINE_ERROR("error : can not set linger scoket opt! failed err(%d)[%s].", errno, getErrInfo());
		}
		else
		{
			LOGI("error : can not set linger scoket opt! failed err(%d)[%s].", errno, getErrInfo());
		}
		return -1;
	}

	// set nonblock

	FD_ZERO(&m_ReadFDs[SELECT_BAK]);
	FD_ZERO(&m_WriteFDs[SELECT_BAK]);
	FD_ZERO(&m_ExceptFDs[SELECT_BAK]);

	m_Timeout[SELECT_BAK].tv_sec = 0;
	m_Timeout[SELECT_BAK].tv_usec = 0;

	m_bNeedSleep = true;

	LOGI("\t*****************************");
	LOGI("\t*  		txNet log			*");
	LOGI("\t*****************************");

	FD_SET(m_SockID, &m_ReadFDs[SELECT_BAK]);
	FD_SET(m_SockID, &m_WriteFDs[SELECT_BAK]);
	FD_SET(m_SockID, &m_ExceptFDs[SELECT_BAK]);

	LOGI("[INetClientImp::Init] OK.");
	return 0;
}

int txNetSystem::deInit()
{
	m_txSock.cleanUp();
//	close(m_SockID);
	m_SockID = -1;
	return 0;
}

int txNetSystem::update(float timeSinceLastFrame)
{
	bool bRet = false;
	bRet = select();

	int nRet = 0;
	nRet = processRead();
	if (RECVFAIL_PACK_ERR == nRet || RECVSUCC_DISCONNECT == nRet)
	{
		return nRet;
	}

	bRet = processWrite();
	if (!bRet)
	{
		return SENDFAIL_SOCK_ERR;
	}

	return nRet;
}

int txNetSystem::sendPacket(txNetPacket &pack)
{
	return m_txSock.sen_packet(pack);

}

int txNetSystem::connect()
{
	m_sockAddr.sin_addr.s_addr = inet_addr(m_host);
	m_sockAddr.sin_port = htons(m_port);
	int nRet = ::connect(m_SockID, (const sockaddr*) &m_sockAddr, sizeof(sockaddr));
	if (nRet == 0)
	{
		LOGI("[txNetSystem::connect] byip succeed.");
		m_txSock.setFD(m_SockID);
		if(NULL != m_pNotify)
		m_pNotify->onConnected();
		return 0;
	}
	else
	{
		m_sockErr = errno;
		ENGINE_ERROR("error : [txNetSystem::connect] byip failed err(%d)[%s].", errno, getErrInfo());
		return -1;
	}
}

int txNetSystem::connect(char *addr, const int& port, const bool& byip, const bool& showError)
{
	bool bRet = false;
	memcpy(m_host, addr, IP_SIZE - 1);
	m_port = port;
	if (byip)
	{
		return connect();
	}
	else
	{
		int nRet = -1;
		hostent *hos = gethostbyname(m_host);
		if (hos == NULL)
		{
			m_sockErr = errno;
			if (showError)
			{
				ENGINE_ERROR("error : [txNetSystem::connect] failed err(%d)[%s].", errno, getErrInfo());
			}
			else
			{
				LOGI("error : [txNetSystem::connect] failed err(%d)[%s].", errno, getErrInfo());
			}
			return -1;
		}
		for (int i = 0; hos->h_addr_list[i]; ++i)
		{
			m_sockAddr.sin_addr = *(in_addr *) hos->h_addr_list[i];
			m_sockAddr.sin_port = htons(m_port);
			nRet = ::connect(m_SockID, (const sockaddr*) &m_sockAddr, sizeof(sockaddr));
			if (nRet == 0)
			{
				m_txSock.setFD(m_SockID);
				if(NULL != m_pNotify)
					m_pNotify->onConnected();
				return 0;
			}
			else
			{
				m_sockErr = errno;
				if (showError)
				{
					ENGINE_ERROR("error : [txNetSystem::connect] (%s:%d) failed err(%d).", addr, port, errno);
				}
				else
				{
					LOGI("error : [txNetSystem::connect] (%s:%d) failed err(%d).", addr, port, errno);
				}
				return -1;
			}
		}
	}
	return -1;
}

char *txNetSystem::getErrInfo()
{
	return strerror(m_sockErr);
}

bool txNetSystem::select()
{
	m_Timeout[SELECT_USE].tv_sec = m_Timeout[SELECT_BAK].tv_sec;
	m_Timeout[SELECT_USE].tv_usec = m_Timeout[SELECT_BAK].tv_usec;

	m_ReadFDs[SELECT_USE] = m_ReadFDs[SELECT_BAK];
	m_WriteFDs[SELECT_USE] = m_WriteFDs[SELECT_BAK];
	m_ExceptFDs[SELECT_USE] = m_ExceptFDs[SELECT_BAK];

	int nRet = ::select(m_SockID + 1, &m_ReadFDs[SELECT_USE], &m_WriteFDs[SELECT_USE], &m_ExceptFDs[SELECT_USE], &m_Timeout[SELECT_USE]);

	if (SOCKET_ERROR == nRet)
	{
		return false;
	}
	return true;
}

int txNetSystem::processRead()
{
	int nRet = 0;
	txNetPacket pack;
	if (FD_ISSET(m_SockID, &m_ReadFDs[SELECT_USE]))
	{
		nRet = m_txSock.rec_packet(pack);

		if (RECVSUCC_DISCONNECT == nRet)
		{
			LOGI("error : [txNetSystem::processRead] RECVSUCC_DISCONNECT code(%d)", m_txSock.getSocketErr());
			m_txSock.cleanUp();
			m_pNotify->onDisconnected();

			return RECVSUCC_DISCONNECT;
		}

		if (nRet == RECVSUCC_NO_ONEPACK)
		{
			m_bNeedSleep = true;
			return RECVSUCC_NO_ONEPACK;
		}
		else if (nRet == RECVFAIL_PACK_ERR)
		{
			LOGI("error : [txNetSystem::processRead]1 Err RECVFAIL_PACK_ERR code(%d)", m_txSock.getSocketErr());
			m_txSock.cleanUp();
			m_pNotify->onDisconnected();

			return RECVFAIL_PACK_ERR;
		}
		else if (nRet == RECVFAIL_SOCK_ERR)
		{
			LOGI("error : [txNetSystem::processRead]2 Err RECVFAIL_SOCK_ERR code(%d)", m_txSock.getSocketErr());
			m_txSock.cleanUp();
			m_pNotify->onDisconnected();
			return RECVFAIL_SOCK_ERR;
		}
		else if (nRet == RECVSUCC_ONE_PACK)
		{
			while (nRet == RECVSUCC_ONE_PACK)
			{
				m_pNotify->onRecvPacket(&pack);
				nRet = m_txSock.get_packet(pack);
			}
			if (nRet == RECVFAIL_PACK_ERR)
			{
				LOGI("error : [txNetSystem::processRead] get_packet ret RECVFAIL_PACK_ERR.");
				m_txSock.cleanUp();
				m_pNotify->onDisconnected();
				return nRet;
			}
			m_bNeedSleep = false;
		}
	}
	else
	{
		m_bNeedSleep = true;
	}
	return RECVSUCC_NO_ONEPACK;
}

bool txNetSystem::processWrite()
{
	if (FD_ISSET(m_SockID, &m_WriteFDs[SELECT_USE]))
	{
		int nRet = m_txSock.send();
		if (nRet == SENDFAIL_SOCK_ERR)
		{
			LOGI("error : [txNetSystem::processWrite()] Err code(%d)", m_txSock.getSocketErr());
			m_txSock.cleanUp();
			m_pNotify->onDisconnected();
			return false;
		}
	}
	return true;
}

bool txNetSystem::processExceptions()
{
	return false;
}