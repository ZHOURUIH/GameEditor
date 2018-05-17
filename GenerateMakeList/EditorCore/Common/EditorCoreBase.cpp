#include "EditorCoreBase.h"
#include "EditorCore.h"

EditorCore* EditorCoreBase::mEditorCore = NULL;
SocketNetManager* EditorCoreBase::mSocketNetManager = NULL;
EventSystem* EditorCoreBase::mEventSystem = NULL;
CommandSystem* EditorCoreBase::mCommandSystem = NULL;
LibCurlDownloadManager* EditorCoreBase::mLibCurlDownloadManager = NULL;

void EditorCoreBase::notifyConstructDone()
{
	if (mEditorCore == NULL)
	{
		mEditorCore = EditorCore::getSingletonPtr();
		mSocketNetManager = mEditorCore->getSocketNetManager();
		mEventSystem = mEditorCore->getEventSystem();
		mCommandSystem = mEditorCore->getCommandSystem();
		mLibCurlDownloadManager = mEditorCore->getLibCurlDownloadManager();
	}
}