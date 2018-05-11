#ifndef _LIB_CURL_PACKET_FACTORY_H_
#define _LIB_CURL_PACKET_FACTORY_H_

#include "txUtility.h"

#include "LibCurlPacket.h"

class LibCurlPacketFactoryBase
{
public:
	LibCurlPacketFactoryBase(const LIB_CURL_PACKET& type, const std::string& url, const HTTP_METHOD& method)
		:
		mType(type),
		mURL(url),
		mMethod(method)
	{}
	virtual ~LibCurlPacketFactoryBase(){}
	virtual LibCurlPacket* createPacket() = 0;
	void destroyPacket(LibCurlPacket* packet)
	{
		TRACE_DELETE(packet);
	}
	const LIB_CURL_PACKET& getType() { return mType; }
	template<typename T>
	static LibCurlPacketFactoryBase* createFactory(const LIB_CURL_PACKET& type, const std::string& url, const HTTP_METHOD& method)
	{
		T* factory = TRACE_NEW(T, factory, type, url, method);
		return factory;
	}
protected:
	LIB_CURL_PACKET mType;
	std::string mURL;
	HTTP_METHOD mMethod;
};

template <class T>
class LibCurlPacketFactory : public LibCurlPacketFactoryBase
{
public:
	LibCurlPacketFactory(const LIB_CURL_PACKET& type, const std::string& url, const HTTP_METHOD& method)
		:
		LibCurlPacketFactoryBase(type, url, method)
	{}
	LibCurlPacket* createPacket()
	{
		T* newPacket = TRACE_NEW(T, newPacket);
		newPacket->setPacketType(mType);
		newPacket->setURL(mURL);
		newPacket->setMethod(mMethod);
		return newPacket;
	}
};

#endif