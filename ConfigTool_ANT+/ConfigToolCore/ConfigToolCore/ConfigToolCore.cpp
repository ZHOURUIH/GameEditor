#include "ConfigToolCore.h"
#include "SocketNetManager.h"
#include "CTEventSystem.h"
#include "txCommandSystem.h"
#include "DataManager.h"
#include "txSerializer.h"
#include "ToolCoreBase.h"

template<> ConfigToolCore* txSingleton<ConfigToolCore>::ms_Singleton = NULL;

ConfigToolCore::ConfigToolCore()
:
mSocketNetManager(NULL),
mEventSystem(NULL),
mCommandSystem(NULL),
mDataManager(NULL),
mInitFlag(false)
{}

void ConfigToolCore::init()
{
	// 创建事件系统
	mEventSystem = TRACE_NEW(CTEventSystem, mEventSystem);
	// 事件系统创建完成后检查是否有错误信息
	checkErrorInfoBuffer();

	mCommandSystem = TRACE_NEW(txCommandSystem, mCommandSystem);
	mDataManager = TRACE_NEW(DataManager, mDataManager);
	mSocketNetManager = TRACE_NEW(SocketNetManager, mSocketNetManager);
	ToolCoreBase::notifyConstructDone();
	
	mCommandSystem->init();
	mDataManager->init();
	mSocketNetManager->init(9124);
}

void ConfigToolCore::update(float elapsedTime)
{
	mEventSystem->update(elapsedTime);
	mCommandSystem->update(elapsedTime);
	mSocketNetManager->update(elapsedTime);
	mDataManager->update(elapsedTime);
}

void ConfigToolCore::destroy()
{
	TRACE_DELETE(mCommandSystem);
	TRACE_DELETE(mDataManager);
	TRACE_DELETE(mSocketNetManager);
	TRACE_DELETE(mEventSystem);
}

void ConfigToolCore::checkErrorInfoBuffer()
{
	int infoCount = mErrorInfoBuffer.size();
	for (int i = 0; i < infoCount; ++i)
	{
		logError(mErrorInfoBuffer[i]);
	}
	mErrorInfoBuffer.clear();
}

void ConfigToolCore::logError(const std::string& info)
{
	if (mEventSystem == NULL)
	{
		mErrorInfoBuffer.push_back(info);
		return;
	}
	// 如果还没有初始化完成,则先将消息放入事件缓冲中
	std::vector<std::string> params;
	params.push_back(info);
	mEventSystem->pushEvent(CET_ERROR_LOG, params, mInitFlag);
}

void ConfigToolCore::logInfo(const std::string& info)
{
	if (mEventSystem == NULL)
	{
		return;
	}
	// 如果还没有初始化完成,则先将消息放入事件缓冲中
	std::vector<std::string> params;
	params.push_back(info);
	mEventSystem->pushEvent(CET_INFO_LOG, params, mInitFlag);
}