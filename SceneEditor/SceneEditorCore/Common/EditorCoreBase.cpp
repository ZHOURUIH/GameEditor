#include "txEngineRoot.h"

#include "EditorCoreBase.h"
#include "EditorCoreRoot.h"

EditorCoreRoot* EditorCoreBase::mEditorCoreRoot = NULL;
ECEditorManager* EditorCoreBase::mEditorManager = NULL;
CoreEventSystem* EditorCoreBase::mCoreEventSystem = NULL;
ECSceneManager* EditorCoreBase::mEditorSceneManager = NULL;
ECUndoManager* EditorCoreBase::mUndoManager = NULL;
SceneEditorCore* EditorCoreBase::mSceneEditorCore = NULL;
ComponentEditorCore* EditorCoreBase::mComponentEditorCore = NULL;

void EditorCoreBase::notifyConstructDone()
{
	if (mEditorCoreRoot == NULL)
	{
		mEditorCoreRoot = EditorCoreRoot::getSingletonPtr();
		mEditorManager = mEditorCoreRoot->getEditorManager();
		mCoreEventSystem = mEditorCoreRoot->getCoreEventSystem();
		mEditorSceneManager = mEditorCoreRoot->getEditorSceneManager();
		mUndoManager = mEditorCoreRoot->getUndoManager();
		mSceneEditorCore = mEditorCoreRoot->getSceneEditorCore();
		mComponentEditorCore = mEditorCoreRoot->getComponentEditorCore();
	}
}