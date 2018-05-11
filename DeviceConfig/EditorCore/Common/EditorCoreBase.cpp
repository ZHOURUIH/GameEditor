#include "EditorCoreBase.h"
#include "EditorCore.h"

EditorCore* EditorCoreBase::mEditorCore = NULL;
SocketNetManager* EditorCoreBase::mSocketNetManager = NULL;
CTEventSystem* EditorCoreBase::mEventSystem = NULL;
txCommandSystem* EditorCoreBase::mCommandSystem = NULL;
txSystemInfo* EditorCoreBase::mSystemInfo = NULL;
LibCurlNetManager* EditorCoreBase::mLibCurlNetManager = NULL;

void EditorCoreBase::notifyConstructDone()
{
	if (mEditorCore == NULL)
	{
		mEditorCore = EditorCore::getSingletonPtr();
		mSocketNetManager = mEditorCore->getSocketNetManager();
		mEventSystem = mEditorCore->getEventSystem();
		mCommandSystem = mEditorCore->getCommandSystem();
		mSystemInfo = mEditorCore->getSystemInfo();
		mLibCurlNetManager = mEditorCore->getLibCurlNetManager();
	}
}