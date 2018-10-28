#ifndef _LIBCURL_PACKET_H_
#define _LIBCURL_PACKET_H_

#include "CommonDefine.h"
#include "ToolCoreBase.h"

class LibcurlPacket : public ToolCoreBase
{
public:
	LibcurlPacket(){}
	virtual ~LibcurlPacket(){}
	// data只包含这个消息包所需要的信息
	virtual void read(char* data, const int& dataSize){}
	// data外部new的空的内存,消息包往data中写入消息内容
	virtual void write(std::string& value) = 0;
	virtual void execute(const std::string& value) = 0;
	const LIBCURL_PACKET& getPacketType() { return mType; }
	const std::string& getURL() { return mURL; }
	const HTTP_METHOD& getMethod(){ return mMethod; }
	void setURL(const std::string& url) { mURL = url; }
	void setPacketType(const LIBCURL_PACKET& type) { mType = type; }
	void setMethod(const HTTP_METHOD& method) { mMethod = method; }
protected:
	LIBCURL_PACKET mType;
	std::string mURL;
	HTTP_METHOD mMethod;
};

#endif