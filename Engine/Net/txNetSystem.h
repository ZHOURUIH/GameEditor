#ifndef __txNetSystem_H__
#define __txNetSystem_H__

#include "txEngineDefine.h"

#if RUN_PLATFORM == PLATFORM_ANDROID
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#elif RUN_PLATFORM == PLATFORM_WINDOWS
#endif

typedef int Socket;
#define SOCKET_ERROR  (-1)
#if RUN_PLATFORM == PLATFORM_ANDROID
#define INVALID_SOCKET 0
#endif

#include "txNetSocket.h"

class txNetNotify;

class txNetSystem
{
public:
	txNetSystem();
	~txNetSystem();

public:
	// return 0 ok
	// return -1 then call getErrInfo for more error info
	int init(txNetNotify *pNotify, const bool& showError = true);

	// return 0 ok
	int deInit();

	// return RECV_CODE(200-205)
	// arg timeSceneLastFrame no use
	int update(float timeSceneLastFrame);

	// return SEND_CODE(100-107)
	int sendPacket(txNetPacket &pack);

	// return 0 ok
	// return -1 then call getErrInfo for more error info
	int connect();
	int connect(char *addr, const int& port, const bool& byip = false, const bool& showError = true);

	char *getErrInfo();

	void setNotify(txNetNotify * pNotify){m_pNotify = pNotify;}
private:
	bool select();
	int processRead();
	bool processWrite();
	bool processExceptions();

private:
	txNetSocket m_txSock;
	Socket m_SockID;
	sockaddr_in m_sockAddr;
	txNetNotify *m_pNotify;

	int m_sockErr;
	char m_host[IP_SIZE];
	int m_port;

	enum
	{
		SELECT_BAK = 0, SELECT_USE = 1, SELECT_MAX = 2
	};
	fd_set m_ReadFDs[SELECT_MAX];
	fd_set m_WriteFDs[SELECT_MAX];
	fd_set m_ExceptFDs[SELECT_MAX];

	timeval m_Timeout[SELECT_MAX];

	bool m_bNeedSleep;
};

#endif