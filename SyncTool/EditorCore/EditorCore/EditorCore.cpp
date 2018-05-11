#include "txMemeryTrace.h"
#include "CommandSystem.h"
#include "txSerializer.h"
#include "SocketNetManager.h"
#include "EventSystem.h"
#include "EditorCore.h"
#include "DownloadManager.h"
#include "UploadManager.h"
#include "File.h"
#include "LibCurlDownloadManager.h"

template<> EditorCore* txSingleton<EditorCore>::ms_Singleton = NULL;

EditorCore::EditorCore()
:
mEventSystem(NULL),
mCommandSystem(NULL),
mDownloadManager(NULL),
mUploadManager(NULL),
mLibCurlDownloadManager(NULL),
mInitFlag(false)
{}

void EditorCore::init()
{
	// 创建事件系统
	mEventSystem = TRACE_NEW(EventSystem, mEventSystem);
	// 事件系统创建完成后检查是否有错误信息
	checkErrorInfoBuffer();

	mCommandSystem = TRACE_NEW(CommandSystem, mCommandSystem);
	mDownloadManager = TRACE_NEW(DownloadManager, mDownloadManager);
	mUploadManager = TRACE_NEW(UploadManager, mUploadManager);
	mLibCurlDownloadManager = TRACE_NEW(LibCurlDownloadManager, mLibCurlDownloadManager);

	EditorCoreBase::notifyConstructDone();
	
	mCommandSystem->init();
	mLibCurlDownloadManager->init();
	mDownloadManager->init();
	mUploadManager->init();
}

void EditorCore::update(float elapsedTime)
{
	mEventSystem->update(elapsedTime);
	mCommandSystem->update(elapsedTime);
	mDownloadManager->update(elapsedTime);
	mUploadManager->update(elapsedTime);
	mLibCurlDownloadManager->update(elapsedTime);
}

void EditorCore::sendEvent(const CORE_EVENT_TYPE& type, const std::string& param, bool sendImmediately)
{
	txVector<std::string> params;
	params.push_back(param);
	sendEvent(type, params, sendImmediately);
}

void EditorCore::sendEvent(const CORE_EVENT_TYPE& type, const txVector<std::string>& params, bool sendImmediately)
{
	// 如果是立即发送的事件,则需要根据初始化标记判断是否应该立即发送
	if (sendImmediately)
	{
		sendImmediately = mInitFlag;
	}
	if (mEventSystem != NULL)
	{
		mEventSystem->pushEvent(type, params, sendImmediately);
	}
}

void EditorCore::sendDelayEvent(const CORE_EVENT_TYPE& type, const std::string& param)
{
	sendEvent(type, param, false);
}

void EditorCore::sendDelayEvent(const CORE_EVENT_TYPE& type, const txVector<std::string>& params)
{
	sendEvent(type, params, false);
}

void EditorCore::destroy()
{
	TRACE_DELETE(mDownloadManager);
	TRACE_DELETE(mUploadManager);
	TRACE_DELETE(mCommandSystem);
	TRACE_DELETE(mEventSystem);
	TRACE_DELETE(mLibCurlDownloadManager);
}

void EditorCore::checkErrorInfoBuffer()
{
	int infoCount = mErrorInfoBuffer.size();
	for (int i = 0; i < infoCount; ++i)
	{
		logError(mErrorInfoBuffer[i]);
	}
	mErrorInfoBuffer.clear();
}

void EditorCore::logError(const std::string& info, const bool& delay)
{
	if (mEventSystem == NULL)
	{
		mErrorInfoBuffer.push_back(info);
		return;
	}
	// 如果还没有初始化完成,则先将消息放入事件缓冲中
	txVector<std::string> params;
	params.push_back(info);
	sendEvent(CET_ERROR_LOG, params, !delay);
}

void EditorCore::logInfo(const std::string& info, const bool& delay)
{
	if (mEventSystem == NULL)
	{
		return;
	}
	// 如果还没有初始化完成,则先将消息放入事件缓冲中
	txVector<std::string> params;
	params.push_back(info);
	sendEvent(CET_INFO_LOG, params, !delay);
}

void EditorCore::requestDownloadVersion()
{
	mDownloadManager->requestDownloadVersion();
}

void EditorCore::requestUpdateGame(const bool& start)
{
	mDownloadManager->startUpdateGame(start);
}

void EditorCore::requestUploadFile(const txVector<std::pair<std::string, std::string>>& fileName)
{
	mUploadManager->uploadFile(fileName);
}