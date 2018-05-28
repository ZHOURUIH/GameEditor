#ifndef _EDITOR_CORE_BASE_H_
#define _EDITOR_CORE_BASE_H_

class EditorCoreRoot;
class CoreEventSystem;
class ECUndoManager;
class ActionTreeEditorCore;
class ComponentEditorCore;
class LayoutEditorCore;
class TremblingEditorCore;
class EditorCoreBase
{
public:
	static void notifyConstructDone();
public:
	static EditorCoreRoot* mEditorCoreRoot;
	static CoreEventSystem* mCoreEventSystem;
	static ECUndoManager* mUndoManager;
	static ActionTreeEditorCore* mActionTreeEditorCore;
	static ComponentEditorCore* mComponentEditorCore;
	static LayoutEditorCore* mLayoutEditorCore;
	static TremblingEditorCore* mTremblingEditorCore;
};

#endif