#include "ToolCoreBase.h"
#include "ConfigToolCore.h"

ConfigToolCore* ToolCoreBase::mConfigToolCore = NULL;
SocketWireless* ToolCoreBase::mSocketWireless = NULL;
CTEventSystem* ToolCoreBase::mEventSystem = NULL;
txCommandSystem* ToolCoreBase::mCommandSystem = NULL;
WirelessDataManager* ToolCoreBase::mWirelessDataManager = NULL;
DataBase* ToolCoreBase::mDataBase = NULL;
DeviceRegisteManager* ToolCoreBase::mDeviceRegisteManager = NULL;
SpeedDataManager* ToolCoreBase::mSpeedDataManager = NULL;
SocketNetManager* ToolCoreBase::mSocketNetManager = NULL;
LibcurlNetManager* ToolCoreBase::mLibcurlNetManager = NULL;
LogSystem* ToolCoreBase::mLogSystem = NULL;

void ToolCoreBase::notifyConstructDone()
{
	if (mConfigToolCore == NULL)
	{
		mConfigToolCore = ConfigToolCore::getSingletonPtr();
		mSocketWireless = mConfigToolCore->getSocketWireless();
		mEventSystem = mConfigToolCore->getEventSystem();
		mCommandSystem = mConfigToolCore->getCommandSystem();
		mWirelessDataManager = mConfigToolCore->getWirelessDataManager();
		mDataBase = mConfigToolCore->getDataBase();
		mDeviceRegisteManager = mConfigToolCore->getDeviceRegisteManager();
		mSpeedDataManager = mConfigToolCore->getSpeedDataManager();
		mSocketNetManager = mConfigToolCore->getSocketNetManager();
		mLibcurlNetManager = mConfigToolCore->getLibcurlNetManager();
		mLogSystem = mConfigToolCore->getLogSystem();
	}
}