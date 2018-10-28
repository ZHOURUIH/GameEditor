#ifndef _LIBCURL_PACKET_FACTORY_H_
#define _LIBCURL_PACKET_FACTORY_H_

#include "Utility.h"
#include "LibCurlPacket.h"

class LibcurlPacketFactoryBase
{
public:
	LibcurlPacketFactoryBase(const LIBCURL_PACKET& type, const std::string& url, const HTTP_METHOD& method)
		:
		mType(type),
		mURL(url),
		mMethod(method)
	{}
	virtual ~LibcurlPacketFactoryBase(){}
	virtual LibcurlPacket* createPacket() = 0;
	void destroyPacket(LibcurlPacket* packet)
	{
		TRACE_DELETE(packet);
	}
	const LIBCURL_PACKET& getType() { return mType; }
	template<typename T>
	static LibcurlPacketFactoryBase* createFactory(const LIBCURL_PACKET& type, const std::string& url, const HTTP_METHOD& method)
	{
		T* factory = TRACE_NEW(T, factory, type, url, method);
		return factory;
	}
protected:
	LIBCURL_PACKET mType;
	std::string mURL;
	HTTP_METHOD mMethod;
};

template <class T>
class LibcurlPacketFactory : public LibcurlPacketFactoryBase
{
public:
	LibcurlPacketFactory(const LIBCURL_PACKET& type, const std::string& url, const HTTP_METHOD& method)
		:
		LibcurlPacketFactoryBase(type, url, method)
	{}
	LibcurlPacket* createPacket()
	{
		T* newPacket = TRACE_NEW(T, newPacket);
		newPacket->setPacketType(mType);
		newPacket->setURL(mURL);
		newPacket->setMethod(mMethod);
		return newPacket;
	}
};

#endif