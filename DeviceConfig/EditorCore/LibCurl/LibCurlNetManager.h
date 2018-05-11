#ifndef _LIB_CURL_NET_MANAGER_H_
#define _LIB_CURL_NET_MANAGER_H_

#include "CommonDefine.h"
#include "curl_windows/curl.h"
#include "LibCurlPacketFactory.h"
#include "txThreadLock.h"

class LibCurlNetManager;
class Args
{
public:
	LibCurlNetManager*	mNetManager;
	LIB_CURL_PACKET		mType;
	CURL*				mCurl;
	char*				mContent;
};

struct OutputElement
{
	LIB_CURL_PACKET mPacket;
	std::string mContent;
	HTTP_METHOD mMethod;
	std::string mURL;
};

class LibCurlPacket;
class LibCurlNetManager
{
public:
	LibCurlNetManager();
	virtual ~LibCurlNetManager();
	virtual void init();
	virtual void update(float elapsedTime);
	void destroy();
	void processOutput();
	void processInput();
	void sendMessage(LibCurlPacket* packet);
	LibCurlPacket* createPacket(const LIB_CURL_PACKET& type);
protected:
	void initFactory();
	static size_t notifyFunction(void *buffer, size_t size, size_t nmemb, void *userp);
	LibCurlPacketFactoryBase* getPacketFactory(const LIB_CURL_PACKET& type);
protected:
	CURLM*			mMultiCurl;
	std::string		mIP;
	txThreadLock	mResultLock;
	txVector<OutputElement>								mOutputList;
	txMap<LIB_CURL_PACKET, LibCurlPacketFactoryBase*>	mPacketFactoryList;
	txVector<std::pair<LIB_CURL_PACKET, std::string>>	mResult;
	
};

#endif