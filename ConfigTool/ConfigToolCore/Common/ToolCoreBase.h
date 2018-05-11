#ifndef _TOOL_CORE_BASE_H_
#define _TOOL_CORE_BASE_H_

class ConfigToolCore;
class SocketNetManager;
class CTEventSystem;
class txCommandSystem;
class SpeedDataManager;
class ToolCoreBase
{
public:
	static void notifyConstructDone();
protected:
	static ConfigToolCore* mConfigToolCore;
	static SocketNetManager* mSocketNetManager;
	static CTEventSystem* mEventSystem;
	static txCommandSystem* mCommandSystem;
	static SpeedDataManager* mSpeedDataManager;
};

#endif