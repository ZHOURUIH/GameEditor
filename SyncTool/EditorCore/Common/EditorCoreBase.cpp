#include "EditorCoreBase.h"
#include "EditorCore.h"

EditorCore* EditorCoreBase::mEditorCore = NULL;
EventSystem* EditorCoreBase::mEventSystem = NULL;
CommandSystem* EditorCoreBase::mCommandSystem = NULL;
DownloadManager* EditorCoreBase::mDownloadManager = NULL;
LibCurlDownloadManager* EditorCoreBase::mLibCurlDownloadManager = NULL;

void EditorCoreBase::notifyConstructDone()
{
	if (mEditorCore == NULL)
	{
		mEditorCore = EditorCore::getSingletonPtr();
		mEventSystem = mEditorCore->getEventSystem();
		mCommandSystem = mEditorCore->getCommandSystem();
		mDownloadManager = mEditorCore->getDownloadManager();
		mLibCurlDownloadManager = mEditorCore->getLibCurlDownloadManager();
	}
}