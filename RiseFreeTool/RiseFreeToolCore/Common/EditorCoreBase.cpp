#include "EditorCoreBase.h"
#include "RiseFreeToolCore.h"

RiseFreeToolCore* EditorCoreBase::mRiseFreeToolCore = NULL;
EventSystem* EditorCoreBase::mEventSystem = NULL;
CommandSystem* EditorCoreBase::mCommandSystem = NULL;
SpeedDataManager* EditorCoreBase::mSpeedDataManager = NULL;
ComManager* EditorCoreBase::mComManager = NULL;

void EditorCoreBase::notifyConstructDone()
{
	if (mRiseFreeToolCore == NULL)
	{
		mRiseFreeToolCore = RiseFreeToolCore::getSingletonPtr();
		mEventSystem = mRiseFreeToolCore->getEventSystem();
		mCommandSystem = mRiseFreeToolCore->getCommandSystem();
		mSpeedDataManager = mRiseFreeToolCore->getSpeedDataManager();
		mComManager = mRiseFreeToolCore->getComManager();
	}
}