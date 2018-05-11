#include "RiseFreeToolCore.h"
#include "SocketNetManager.h"
#include "txMemeryTrace.h"
#include "EventSystem.h"
#include "CommandSystem.h"
#include "SpeedDataManager.h"
#include "SocketPacketFriction.h"
#include "txSerializer.h"
#include "ComManager.h"

template<> RiseFreeToolCore* txSingleton<RiseFreeToolCore>::ms_Singleton = NULL;

RiseFreeToolCore::RiseFreeToolCore()
:
mEventSystem(NULL),
mCommandSystem(NULL),
mSpeedDataManager(NULL),
mComManager(NULL),
mInitFlag(false)
{}

void RiseFreeToolCore::init()
{
	// 创建事件系统
	mEventSystem = TRACE_NEW(EventSystem, mEventSystem);
	// 事件系统创建完成后检查是否有错误信息
	checkErrorInfoBuffer();

	mCommandSystem = TRACE_NEW(CommandSystem, mCommandSystem);
	mSpeedDataManager = TRACE_NEW(SpeedDataManager, mSpeedDataManager);
	mComManager = TRACE_NEW(ComManager, mComManager);

	EditorCoreBase::notifyConstructDone();
	
	mCommandSystem->init();
	mSpeedDataManager->init();
	mComManager->init();
}

void RiseFreeToolCore::update(float elapsedTime)
{
	mEventSystem->update(elapsedTime);
	mCommandSystem->update(elapsedTime);
	mSpeedDataManager->update(elapsedTime);
	mComManager->update(elapsedTime);
}

void RiseFreeToolCore::destroy()
{
	TRACE_DELETE(mComManager);
	TRACE_DELETE(mCommandSystem);
	TRACE_DELETE(mSpeedDataManager);
	TRACE_DELETE(mEventSystem);
}

void RiseFreeToolCore::checkErrorInfoBuffer()
{
	int infoCount = mErrorInfoBuffer.size();
	for (int i = 0; i < infoCount; ++i)
	{
		logError(mErrorInfoBuffer[i]);
	}
	mErrorInfoBuffer.clear();
}

void RiseFreeToolCore::sendEvent(const CORE_EVENT_TYPE& type, const std::string& param, bool sendImmediately)
{
	txVector<std::string> params;
	params.push_back(param);
	sendEvent(type, params, sendImmediately);
}

void RiseFreeToolCore::sendEvent(const CORE_EVENT_TYPE& type, const txVector<std::string>& params, bool sendImmediately)
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

void RiseFreeToolCore::sendDelayEvent(const CORE_EVENT_TYPE& type, const std::string& param)
{
	sendEvent(type, param, false);
}

void RiseFreeToolCore::sendDelayEvent(const CORE_EVENT_TYPE& type, const txVector<std::string>& params)
{
	sendEvent(type, params, false);
}

void RiseFreeToolCore::logError(const std::string& info, const bool& delay)
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

void RiseFreeToolCore::logInfo(const std::string& info, const bool& delay)
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