#ifndef _TOOL_CORE_BASE_H_
#define _TOOL_CORE_BASE_H_

class ConfigToolCore;
class SocketWireless;
class CTEventSystem;
class txCommandSystem;
class WirelessDataManager;
class DataBase;
class DeviceRegisteManager;
class SpeedDataManager;
class SocketNetManager;
class LibcurlNetManager;
class LogSystem;
class ToolCoreBase
{
public:
	static void notifyConstructDone();
public:
	static ConfigToolCore* mConfigToolCore;
	static SocketWireless* mSocketWireless;
	static CTEventSystem* mEventSystem;
	static txCommandSystem* mCommandSystem;
	static WirelessDataManager* mWirelessDataManager;
	static DataBase* mDataBase;
	static DeviceRegisteManager* mDeviceRegisteManager;
	static SpeedDataManager* mSpeedDataManager;
	static SocketNetManager* mSocketNetManager;
	static LibcurlNetManager* mLibcurlNetManager;
	static LogSystem* mLogSystem;
};

#endif