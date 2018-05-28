#ifndef _EDITOR_CORE_BASE_H_
#define _EDITOR_CORE_BASE_H_

class EditorCoreRoot;
class ECEditorManager;
class CoreEventSystem;
class ECSceneManager;
class ECUndoManager;
class SceneEditorCore;
class ComponentEditorCore;
class EditorCoreBase
{
public:
	static void notifyConstructDone();
protected:
	static EditorCoreRoot* mEditorCoreRoot;
	static ECEditorManager* mEditorManager;
	static CoreEventSystem* mCoreEventSystem;
	static ECSceneManager* mEditorSceneManager;
	static ECUndoManager* mUndoManager;
	static SceneEditorCore* mSceneEditorCore;
	static ComponentEditorCore* mComponentEditorCore;
};

#endif