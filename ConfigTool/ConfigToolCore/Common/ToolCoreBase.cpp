#include "ToolCoreBase.h"
#include "ConfigToolCore.h"

ConfigToolCore* ToolCoreBase::mConfigToolCore = NULL;
SocketNetManager* ToolCoreBase::mSocketNetManager = NULL;
CTEventSystem* ToolCoreBase::mEventSystem = NULL;
txCommandSystem* ToolCoreBase::mCommandSystem = NULL;
SpeedDataManager* ToolCoreBase::mSpeedDataManager = NULL;

void ToolCoreBase::notifyConstructDone()
{
	if (mConfigToolCore == NULL)
	{
		mConfigToolCore = ConfigToolCore::getSingletonPtr();
		mSocketNetManager = mConfigToolCore->getSocketNetManager();
		mEventSystem = mConfigToolCore->getEventSystem();
		mCommandSystem = mConfigToolCore->getCommandSystem();
		mSpeedDataManager = mConfigToolCore->getSpeedDataManager();
	}
}