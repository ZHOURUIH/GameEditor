#include "txMemeryTrace.h"
#include "txCommandSystem.h"
#include "txSerializer.h"
#include "txSystemInfo.h"
#include "CTEventSystem.h"
#include "EditorCore.h"
#include "EditorCoreBase.h"

template<> EditorCore* txSingleton<EditorCore>::ms_Singleton = NULL;

EditorCore::EditorCore()
:
mEventSystem(NULL),
mCommandSystem(NULL),
mInitFlag(false)
{}

void EditorCore::init()
{
	// 创建事件系统
	mEventSystem = TRACE_NEW(CTEventSystem, mEventSystem);
	// 事件系统创建完成后检查是否有错误信息
	checkErrorInfoBuffer();

	mCommandSystem = TRACE_NEW(txCommandSystem, mCommandSystem);
	mSystemInfo = TRACE_NEW(txSystemInfo, mSystemInfo);

	EditorCoreBase::notifyConstructDone();
	
	mSystemInfo->init();
	mCommandSystem->init();
}

void EditorCore::update(float elapsedTime)
{
	mEventSystem->update(elapsedTime);
	mCommandSystem->update(elapsedTime);
}

void EditorCore::destroy()
{
	TRACE_DELETE(mCommandSystem);
	TRACE_DELETE(mEventSystem);
	TRACE_DELETE(mSystemInfo);
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

void EditorCore::logError(const std::string& info)
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

void EditorCore::logInfo(const std::string& info)
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