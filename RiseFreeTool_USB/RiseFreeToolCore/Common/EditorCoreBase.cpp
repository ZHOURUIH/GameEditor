#include "EditorCoreBase.h"
#include "RiseFreeToolCore.h"

RiseFreeToolCore* EditorCoreBase::mRiseFreeToolCore = NULL;
EventSystem* EditorCoreBase::mEventSystem = NULL;
CommandSystem* EditorCoreBase::mCommandSystem = NULL;
SpeedDataManager* EditorCoreBase::mSpeedDataManager = NULL;
USBManager* EditorCoreBase::mUSBManager = NULL;

void EditorCoreBase::notifyConstructDone()
{
	if (mRiseFreeToolCore == NULL)
	{
		mRiseFreeToolCore = RiseFreeToolCore::getSingletonPtr();
		mEventSystem = mRiseFreeToolCore->getEventSystem();
		mCommandSystem = mRiseFreeToolCore->getCommandSystem();
		mSpeedDataManager = mRiseFreeToolCore->getSpeedDataManager();
		mUSBManager = mRiseFreeToolCore->getUSBManager();
	}
}