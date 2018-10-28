#include "LibcurlNetManager.h"

#include "json/json.h"
#include "json/reader.h"
#include "json/value.h"
#include "Utility.h"
#include "LibcurlPacket.h"
#include "LibcurlPacketFactory.h"
#include "LibcurlPacketHeader.h"

#define ADD_LIBCURL_FACTROY(type, packet, url, method)\
{\
	LibcurlPacketFactoryBase* factroy = LibcurlPacketFactoryBase::createFactory<LibcurlPacketFactory<type>>(packet, url, method); \
	mPacketFactoryList.insert(packet, factroy); \
}

LibcurlNetManager::LibcurlNetManager()
{
	mIP = "http://app1.taxingtianji.com";
	mMultiCurl = NULL;
}

LibcurlNetManager::~LibcurlNetManager()
{
	destroy();
}

void LibcurlNetManager::init()
{
	curl_global_init(CURL_GLOBAL_ALL);
	mMultiCurl = curl_multi_init();
	curl_multi_setopt(mMultiCurl, CURLMOPT_MAXCONNECTS, 8);
	// 初始化工厂
	initFactory();
}

void LibcurlNetManager::destroy()
{
	curl_multi_cleanup(mMultiCurl);
	curl_global_cleanup();
	mMultiCurl = NULL;
}

void LibcurlNetManager::initFactory()
{
	ADD_LIBCURL_FACTROY(SetupDevice, LP_SETUP_DEVICE, "/wechat/php/wirelessData.php?", HM_POST);
}

void LibcurlNetManager::update(float elapsedTime)
{
	processInput();
	processOutput();
	int still_running = 0;
	do
	{
		curl_multi_perform(mMultiCurl, &still_running);
		if (still_running > 0)
		{
			fd_set fdread, fdwrite, fdexcp;
			int maxfd = -1;
			FD_ZERO(&fdread);
			FD_ZERO(&fdwrite);
			FD_ZERO(&fdexcp);
			CURLMcode ret = curl_multi_fdset(mMultiCurl, &fdread, &fdwrite, &fdexcp, &maxfd);
			if (ret != CURLM_OK)
			{
				TOOL_CORE_ERROR("curl_multi_fdset");
				break;
			}
			long timeout = 0;
			ret = curl_multi_timeout(mMultiCurl, &timeout);
			if (ret != CURLM_OK)
			{
				TOOL_CORE_ERROR("curl_multi_timeout");
				break;
			}
			if (timeout == -1)
			{
				timeout = 100;
			}
			if (maxfd >= 0)
			{
				timeval T;
				T.tv_sec = timeout / 1000;
				T.tv_usec = (timeout % 1000) * 1000;
				// 执行监听，当文件描述符状态发生改变的时候返回
				// 返回0，程序调用curl_multi_perform通知curl执行相应操作
				// 返回-1，表示select错误
				if (0 > select(maxfd + 1, &fdread, &fdwrite, &fdexcp, &T))
				{
					TOOL_CORE_ERROR("E: select(%i,,,,%li): %i: %s", maxfd + 1, timeout, errno, strerror(errno));
					break;
				}
			}
		}
		CURLMsg* msg;
		int msgs_left;
		while ((msg = curl_multi_info_read(mMultiCurl, &msgs_left)))
		{
			if (msg->msg == CURLMSG_DONE)
			{
				char *url;
				CURL *e = msg->easy_handle;
				curl_easy_getinfo(msg->easy_handle, CURLINFO_PRIVATE, &url);
				// 当一个easy handler传输完成，此easy handler仍然仍然停留在multi stack中,
				// 调用curl_multi_remove_handle将其从multi stack中移除,然后调用curl_easy_cleanup将其关闭
				curl_multi_remove_handle(mMultiCurl, e);
				curl_easy_cleanup(e);
			}
			else
			{
				TOOL_CORE_ERROR("CURLMsg (%d)", msg->msg);
			}
		}
	} while (still_running > 0);
}

void LibcurlNetManager::processOutput()
{
	int outPutStreamSize = mOutputList.size();
	FOR_STL(mOutputList, int i = 0; i < outPutStreamSize; ++i)
	{
		CURL* curl = curl_easy_init();
		Args* args = TRACE_NEW(Args, args);
		args->mNetManager = this;
		args->mType = mOutputList[i].mPacket;
		args->mCurl = curl;
		args->mContent = NULL;
		std::string url = mIP + mOutputList[i].mURL;
		if (mOutputList[i].mMethod == HM_GET)
		{
			url += mOutputList[i].mContent;
		}
		else
		{
			curl_easy_setopt(curl, CURLOPT_POST, 1);
			std::string content = mOutputList[i].mContent;
			args->mContent = TRACE_NEW_ARRAY(char, content.length() + 1, args->mContent);
			args->mContent[content.length()] = 0;
			memcpy(args->mContent, content.c_str(), content.length());
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, args->mContent);
		}
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, notifyFunction);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)args);
		curl_easy_setopt(curl, CURLOPT_HEADER, 0);
		if (CURLM_OK != curl_multi_add_handle(mMultiCurl, curl))
		{
			TOOL_CORE_ERROR("curl return value error");
		}
	}
	END_FOR_STL(mOutputList);
	mOutputList.clear();
}

void LibcurlNetManager::processInput()
{
	LOCK(mResultLock);
	int sizeResult = mResult.size();
	// 取得所有的JSON返回信息
	FOR_STL(mResult, int i = 0; i < sizeResult; ++i)
	{
		// 创建相应的报处理收到的数据
		LibcurlPacketFactoryBase* factorFind = getPacketFactory(mResult[i].first);
		LibcurlPacket* packetReply = factorFind->createPacket();
		packetReply->execute(mResult[i].second);
		factorFind->destroyPacket(packetReply);
	}
	END_FOR_STL(mResult);
	mResult.clear();
	UNLOCK(mResultLock);
}

LibcurlPacketFactoryBase* LibcurlNetManager::getPacketFactory(const LIBCURL_PACKET& type)
{
	txMap<LIBCURL_PACKET, LibcurlPacketFactoryBase*>::iterator iter = mPacketFactoryList.find(type);
	if (iter == mPacketFactoryList.end())
	{
		return NULL;
	}
	return iter->second;
}

void LibcurlNetManager::sendMessage(LibcurlPacket* packet)
{
	// 将包的数据存入列表
	std::string packetData;
	packet->write(packetData);
	// 保存发送数据
	OutputElement outElement;
	outElement.mPacket = packet->getPacketType();
	outElement.mContent = StringUtility::ANSIToUTF8(packetData);
	outElement.mURL = packet->getURL();
	outElement.mMethod = packet->getMethod();
	mOutputList.push_back(outElement);
	getPacketFactory(packet->getPacketType())->destroyPacket(packet);
}

LibcurlPacket* LibcurlNetManager::createPacket(const LIBCURL_PACKET& type)
{
	LibcurlPacketFactoryBase* factroy = getPacketFactory(type);
	if (factroy == NULL)
	{
		return NULL;
	}
	return factroy->createPacket();
}

size_t LibcurlNetManager::notifyFunction(void *buffer, size_t size, size_t nmemb, void *userp)
{
	Args* arg = (Args*)userp;
	TRACE_DELETE_ARRAY(arg->mContent);
	LibcurlNetManager* netMgr = arg->mNetManager;
	// 处理JSON返回数据
	LOCK(netMgr->mResultLock);
	netMgr->mResult.push_back(std::make_pair(arg->mType, (char*)buffer));
	TRACE_DELETE(arg);
	UNLOCK(netMgr->mResultLock);
	return size * nmemb;
}