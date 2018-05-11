#ifndef _TOOL_CORE_BASE_H_
#define _TOOL_CORE_BASE_H_

class RiseFreeToolCore;
class EventSystem;
class CommandSystem;
class SpeedDataManager;
class USBManager;
class EditorCoreBase
{
public:
	static void notifyConstructDone();
protected:
	static RiseFreeToolCore* mRiseFreeToolCore;
	static EventSystem* mEventSystem;
	static CommandSystem* mCommandSystem;
	static SpeedDataManager* mSpeedDataManager;
	static USBManager* mUSBManager;
};

#endif