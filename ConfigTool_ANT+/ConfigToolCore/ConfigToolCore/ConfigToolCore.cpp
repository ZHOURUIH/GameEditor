#include "ConfigToolCore.h"
#include "SocketWireless.h"
#include "CTEventSystem.h"
#include "txCommandSystem.h"
#include "WirelessDataManager.h"
#include "txSerializer.h"
#include "ToolCoreBase.h"
#include "DataBase.h"
#include "DeviceRegisteManager.h"
#include "SpeedDataManager.h"
#include "SocketNetManager.h"
#include "LibcurlNetManager.h"
#include "LogSystem.h"
#include "GameConfig.h"

template<> ConfigToolCore* txSingleton<ConfigToolCore>::ms_Singleton = NULL;

ConfigToolCore::ConfigToolCore()
:
mInitFlag(false)
{
	// 先创建事件系统
	mEventSystem = TRACE_NEW(CTEventSystem, mEventSystem);
	mDataBase = TRACE_NEW(DataBase, mDataBase);
	mCommandSystem = TRACE_NEW(txCommandSystem, mCommandSystem);
	mWirelessDataManager = TRACE_NEW(WirelessDataManager, mWirelessDataManager);
	mSocketWireless = TRACE_NEW(SocketWireless, mSocketWireless);
	mDeviceRegisteManager = TRACE_NEW(DeviceRegisteManager, mDeviceRegisteManager);
	mSpeedDataManager = TRACE_NEW(SpeedDataManager, mSpeedDataManager);
	mSocketNetManager = TRACE_NEW(SocketNetManager, mSocketNetManager);
	mLibcurlNetManager = TRACE_NEW(LibcurlNetManager, mLibcurlNetManager);
	mLogSystem = TRACE_NEW(LogSystem, mLogSystem);
	mGameConfig = TRACE_NEW(GameConfig, mGameConfig);
	ToolCoreBase::notifyConstructDone();
}

void ConfigToolCore::init()
{
	// 检查是否有错误信息
	checkErrorInfoBuffer();
	mGameConfig->init();
	mDataBase->init();
	mCommandSystem->init();
	mWirelessDataManager->init();
	mSocketWireless->init(9124);
	mDeviceRegisteManager->init();
	mSpeedDataManager->init();
	mSocketNetManager->init(60060, 5000);
	mLibcurlNetManager->init();
	mLogSystem->init();
}

void ConfigToolCore::update(float elapsedTime)
{
	mEventSystem->update(elapsedTime);
	mCommandSystem->update(elapsedTime);
	mSocketWireless->update(elapsedTime);
	mWirelessDataManager->update(elapsedTime);
	mSpeedDataManager->update(elapsedTime);
	mSocketNetManager->update(elapsedTime);
	mLibcurlNetManager->update(elapsedTime);
}

void ConfigToolCore::destroy()
{
	TRACE_DELETE(mGameConfig);
	TRACE_DELETE(mCommandSystem);
	TRACE_DELETE(mWirelessDataManager);
	TRACE_DELETE(mSocketWireless);
	TRACE_DELETE(mEventSystem);
	TRACE_DELETE(mDataBase);
	TRACE_DELETE(mDeviceRegisteManager);
	TRACE_DELETE(mSpeedDataManager);
	TRACE_DELETE(mSocketNetManager);
	TRACE_DELETE(mLibcurlNetManager);
	TRACE_DELETE(mLogSystem);
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

void ConfigToolCore::sendEvent(const CORE_EVENT& type, const std::string& param, const bool& sendImmediately)
{
	txVector<std::string> params;
	params.push_back(param);
	sendEvent(type, params, sendImmediately);
}

void ConfigToolCore::sendEvent(const CORE_EVENT& type, txVector<std::string>& params, bool sendImmediately)
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

void ConfigToolCore::sendDelayEvent(const CORE_EVENT& type, const std::string& param)
{
	sendEvent(type, param, false);
}

void ConfigToolCore::sendDelayEvent(const CORE_EVENT& type, txVector<std::string>& params)
{
	sendEvent(type, params, false);
}

void ConfigToolCore::logError(const std::string& info)
{
	// 如果事件系统还没有创建,则先放入错误信息缓冲中
	if (mEventSystem == NULL)
	{
		mErrorInfoBuffer.push_back(info);
		return;
	}
	sendEvent(CE_ERROR_LOG, info);
}

void ConfigToolCore::logInfo(const std::string& info)
{
	sendEvent(CE_INFO_LOG, info);
}

void ConfigToolCore::setDataPath(const std::string& path)
{
	std::string newPath = path;
	if (!StringUtility::endWith(newPath, "/"))
	{
		newPath += "/";
	}
	mDataBase->setDataPath(newPath, true);
}