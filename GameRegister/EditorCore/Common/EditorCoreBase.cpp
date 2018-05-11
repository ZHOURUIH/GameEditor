#include "EditorCoreBase.h"
#include "EditorCore.h"

EditorCore* EditorCoreBase::mEditorCore = NULL;
CTEventSystem* EditorCoreBase::mEventSystem = NULL;
txCommandSystem* EditorCoreBase::mCommandSystem = NULL;
txSystemInfo* EditorCoreBase::mSystemInfo = NULL;

void EditorCoreBase::notifyConstructDone()
{
	if (mEditorCore == NULL)
	{
		mEditorCore = EditorCore::getSingletonPtr();
	}
	if (mEventSystem == NULL)
	{
		mEventSystem = mEditorCore->getEventSystem();
	}
	if (mCommandSystem == NULL)
	{
		mCommandSystem = mEditorCore->getCommandSystem();
	}
	if (mSystemInfo == NULL)
	{
		mSystemInfo = mEditorCore->getSystemInfo();
	}
}