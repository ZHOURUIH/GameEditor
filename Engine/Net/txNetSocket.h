#ifndef __txNetSocket_H__
#define __txNetSocket_H__

#include "txNetProtocol.h"
#include "txEngineDefine.h"

#define IP_SIZE					18
#define SENDBUFF_SIZE			TOTAL_PACKET_SIZE*20
#define RECVBUFF_SIZE			TOTAL_PACKET_SIZE*2

enum SEND_CODE
{
	SENDFAIL_PACK_ERR = 100, // 发送失败,包异常
	SENDFAIL_SOCK_ERR = 101, // 发送失败，SOCKET异常
	SENDFAIL_FULL_BUF = 102, // 发送失败，本地缓冲区满
	SENDSUCC_ADD_PACK = 103, // 成功添加到本地缓冲区
	SENDSUCC_NO_BUFF = 104, // 本地缓冲区空
	SENDSUCC_EAGIN = 105, // 发送直到eagin
	SENDSUCC_ONE_PACK = 106, // 成功发送一个包
	SENDFAIL_DISCONNECT = 107 // 连接断开
};

enum RECV_CODE
{
	RECVFAIL_PACK_ERR = 200, // 接收失败，包异常(需断开当前连接)
	RECVFAIL_SOCK_ERR = 201, // 接收失败，SOCKET异常
	RECVSUCC_ONE_PACK = 202, // 成功接收一个包
	RECVSUCC_NO_ONEPACK = 203, // 接收数据不足一个包
	RECVSUCC_DISCONNECT = 204, // 客户端断开连接
	RECVSUCC_EAGIN = 205 // 接受直到eagin
};

class txNetSocket
{
public:
	txNetSocket();
	~txNetSocket();

public:
	int sen_packet(txNetPacket &pack);
	int rec_packet(txNetPacket &pack);
	int get_packet(txNetPacket &pack);

public:
	int send();
	void setFD(const int& fd){ m_fd = fd; }
	bool isSocketErr(const int& flag);
	int getSocketErr();
	void cleanUp();
	const int& getSendCount() { return m_senCount; }

private:
	char m_host[IP_SIZE];
	int m_port;

	int m_sockErr;
	int m_fd;
	int m_rcvCount; /// keep recv buffer bytes number
	char m_rcvBuf[RECVBUFF_SIZE]; /// recv buffer

	int m_senCount; /// keep send buffer bytes number
	char m_sndBuf[SENDBUFF_SIZE]; /// send buffer
};

#endif