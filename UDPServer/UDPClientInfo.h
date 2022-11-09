#ifndef _UDP_CLIENT_INFO_H_
#define _UDP_CLIENT_INFO_H_

#include "Utility.h"
#include "Vector.h"
#include "Map.h"

// udp通信的单个客户端的信息
class UDPClientInfo : public Utility
{
protected:
	Vector<pair<char*, int>> mSendList;	// 待发送的消息列表
	sockaddr_in mAddress;				// 发送的地址
	llong mToken = 0;					// 客户端凭证,一般是角色ID
public:
	~UDPClientInfo();
	llong getToken() const							{ return mToken; }
	const sockaddr_in& getAddress() const			{ return mAddress; }
	const Vector<pair<char*, int>>& getSendList() const { return mSendList; }
	void setToken(const llong token)				{ mToken = token; }
	void setAddress(const sockaddr_in& address)		{ mAddress = address; }
	void addSendData(char* data, const int length)	{ mSendList.push_back(make_pair(data, length)); }
	void clear();
};

#endif