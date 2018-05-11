#include "txEngineDefine.h"

#if RUN_PLATFORM == PLATFORM_ANDROID
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

#include "txNetSocket.h"
#include "txEngineLog.h"
#include "Utility.h"
#include "txEngineRoot.h"

txNetSocket::txNetSocket() :
		m_port(0), m_sockErr(0), m_rcvCount(0), m_senCount(0)
{
	memset(m_host, 0, IP_SIZE);
	memset(m_sndBuf, 0, SENDBUFF_SIZE);
	memset(m_rcvBuf, 0, RECVBUFF_SIZE);
}

txNetSocket::~txNetSocket()
{

}

int txNetSocket::sen_packet(txNetPacket &pack)
{
	if (pack.length > CONTENTS_SIZE)
	{
		return SENDFAIL_PACK_ERR;
	}

	if (SENDBUFF_SIZE - m_senCount < PACK_HEADER_SIZE + pack.length)
	{
		return SENDFAIL_FULL_BUF;
	}

	memcpy(m_sndBuf + m_senCount, &pack, PACK_HEADER_SIZE + pack.length);

	m_senCount += PACK_HEADER_SIZE + pack.length;

	return SENDSUCC_ADD_PACK;
}

int txNetSocket::rec_packet(txNetPacket &pack)
{
	int nRet = recv(m_fd, m_rcvBuf + m_rcvCount, TOTAL_PACKET_SIZE, 0);

	if (0 == nRet)
	{
		return RECVSUCC_DISCONNECT;
	}
	else if (-1 == nRet)
	{
		m_sockErr = errno;		// mark errno

		if (errno == EPIPE)
		{
			ENGINE_ERROR("error : [txNetSocket::rec_packet] EPIPE");
			return RECVSUCC_DISCONNECT;
		}
		if (errno == EAGAIN)
		{
			ENGINE_ERROR("error : [txNetSocket::rec_packet] EAGAIN");
			return RECVSUCC_EAGIN;
		}
		// return err
		return RECVFAIL_SOCK_ERR;
	}

	m_rcvCount += nRet;

	if (m_rcvCount < PACK_HEADER_SIZE)
	{
		return RECVSUCC_NO_ONEPACK;
	}
	else
	{
		memcpy(&pack.length, m_rcvBuf + LENGTH_INDEX, LENGTH_SIZE);
		if (pack.length > CONTENTS_SIZE)
		{
			return RECVFAIL_PACK_ERR;
		}
		if (pack.length + PACK_HEADER_SIZE > m_rcvCount)
		{
			return RECVSUCC_NO_ONEPACK;
		}

		memcpy(&pack.header, m_rcvBuf + HEADER_INDEX, HEADER_SIZE);
		memcpy(&pack.command, m_rcvBuf + COMMAND_INDEX, COMMAND_SIZE);
		memcpy(&pack.contents, m_rcvBuf + CONTENTS_INDEX, pack.length);

		// copy remain buffer of receive buffer to head of if
		// need think deep of this operation
		memmove(m_rcvBuf + HEADER_INDEX, m_rcvBuf + PACK_HEADER_SIZE + pack.length,
		RECVBUFF_SIZE - PACK_HEADER_SIZE - pack.length);

		m_rcvCount -= PACK_HEADER_SIZE + pack.length;

		return RECVSUCC_ONE_PACK;
	}
}

int txNetSocket::get_packet(txNetPacket &pack)
{
	if (m_rcvCount < PACK_HEADER_SIZE ||  NULL == m_rcvBuf)
	{
		return RECVSUCC_NO_ONEPACK;
	}
	else
	{
		memcpy(&pack.length, m_rcvBuf + LENGTH_INDEX, LENGTH_SIZE);
		if (pack.length > CONTENTS_SIZE)
		{
			return RECVFAIL_PACK_ERR;
		}
		if (pack.length + PACK_HEADER_SIZE > m_rcvCount)
		{
			return RECVSUCC_NO_ONEPACK;
		}
		memcpy(&pack.header, m_rcvBuf + HEADER_INDEX, HEADER_SIZE);
		memcpy(&pack.command, m_rcvBuf + COMMAND_INDEX, COMMAND_SIZE);
		memcpy(&pack.contents, m_rcvBuf + CONTENTS_INDEX, pack.length);

		// copy remain buffer of receive buffer to head of if
		// need think deep of this operation
		memmove(m_rcvBuf + HEADER_INDEX, m_rcvBuf + PACK_HEADER_SIZE + pack.length,
		RECVBUFF_SIZE - PACK_HEADER_SIZE - pack.length);

		m_rcvCount -= PACK_HEADER_SIZE + pack.length;

		return RECVSUCC_ONE_PACK;
	}
}

int txNetSocket::send()
{
	while (true)
	{
		if (m_senCount <= 0)
		{
			return SENDSUCC_NO_BUFF;
		}

		int sent = 0;
		sent = ::send(m_fd, m_sndBuf, m_senCount, 0);

		if (sent == 0)
		{
			ENGINE_ERROR("error : [txNetSocket::send] send no buff");
			return SENDSUCC_NO_BUFF;
		}
		if (-1 == sent)
		{
			if (errno == EAGAIN)
			{
				ENGINE_ERROR("error : [txNetSocket::send] send EAGAIN");
				return SENDSUCC_EAGIN;
			}
			if (errno == EPIPE)
			{
				ENGINE_ERROR("error : [txNetSocket::send]send EPIPE");
				return SENDFAIL_DISCONNECT;
			}
			return SENDFAIL_DISCONNECT;
		}

		if (sent != m_senCount)
		{
			memmove(m_sndBuf + HEADER_INDEX, m_sndBuf + sent, m_senCount - sent);
		}

		m_senCount -= sent;
	}
	return SENDSUCC_ONE_PACK;
}

bool txNetSocket::isSocketErr(const int& flag)
{
	if (-1 == flag)
	{
		m_sockErr = errno;
		return true;
	}
	return false;
}

int txNetSocket::getSocketErr()
{
	return m_sockErr;
}

void txNetSocket::cleanUp()
{
	m_port = 0;
	m_sockErr = 0;
	m_rcvCount = 0;
	m_senCount = 0;
	memset(m_host, 0, IP_SIZE);
	memset(m_sndBuf, 0, SENDBUFF_SIZE);
	memset(m_rcvBuf, 0, RECVBUFF_SIZE);
#if RUN_PLATFORM == PLATFORM_WINDOWS
	closesocket(m_fd);
#elif RUN_PLATFORM == PLATFORM_ANDROID
	close(m_fd);
#endif
}
