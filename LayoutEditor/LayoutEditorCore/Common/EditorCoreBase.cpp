#include "EditorCoreBase.h"
#include "EditorCoreRoot.h"

EditorCoreRoot* EditorCoreBase::mEditorCoreRoot = NULL;
CoreEventSystem* EditorCoreBase::mCoreEventSystem = NULL;
ECUndoManager* EditorCoreBase::mUndoManager = NULL;
ActionTreeEditorCore* EditorCoreBase::mActionTreeEditorCore = NULL;
ComponentEditorCore* EditorCoreBase::mComponentEditorCore = NULL;
LayoutEditorCore* EditorCoreBase::mLayoutEditorCore = NULL;
TremblingEditorCore* EditorCoreBase::mTremblingEditorCore = NULL;

void EditorCoreBase::notifyConstructDone()
{
	if (mEditorCoreRoot == NULL)
	{
		mEditorCoreRoot = EditorCoreRoot::getSingletonPtr();
		mCoreEventSystem = mEditorCoreRoot->getCoreEventSystem();
		mUndoManager = mEditorCoreRoot->getUndoManager();
		mActionTreeEditorCore = mEditorCoreRoot->getActionTreeEditorCore();
		mComponentEditorCore = mEditorCoreRoot->getComponentEditorCore();
		mLayoutEditorCore = mEditorCoreRoot->getLayoutEditorCore();
		mTremblingEditorCore = mEditorCoreRoot->getTremblingEditorCore();
	}
}