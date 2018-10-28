#ifndef _LIB_CURL_NET_MANAGER_H_
#define _LIB_CURL_NET_MANAGER_H_

#include "CommonDefine.h"
#include "curl_windows/curl.h"
#include "LibCurlPacketFactory.h"
#include "ThreadLock.h"

class LibcurlNetManager;
class Args
{
public:
	LibcurlNetManager*	mNetManager;
	LIBCURL_PACKET		mType;
	CURL*				mCurl;
	char*				mContent;
};

struct OutputElement
{
	LIBCURL_PACKET mPacket;
	std::string mContent;
	HTTP_METHOD mMethod;
	std::string mURL;
};

class LibcurlPacket;
class LibcurlNetManager
{
public:
	LibcurlNetManager();
	virtual ~LibcurlNetManager();
	virtual void init();
	virtual void update(float elapsedTime);
	void destroy();
	void processOutput();
	void processInput();
	void sendMessage(LibcurlPacket* packet);
	LibcurlPacket* createPacket(const LIBCURL_PACKET& type);
protected:
	void initFactory();
	static size_t notifyFunction(void *buffer, size_t size, size_t nmemb, void *userp);
	LibcurlPacketFactoryBase* getPacketFactory(const LIBCURL_PACKET& type);
protected:
	CURLM*		mMultiCurl;
	std::string	mIP;
	ThreadLock	mResultLock;
	txVector<OutputElement>								mOutputList;
	txMap<LIBCURL_PACKET, LibcurlPacketFactoryBase*>	mPacketFactoryList;
	txVector<std::pair<LIBCURL_PACKET, std::string>>	mResult;
	
};

#endif