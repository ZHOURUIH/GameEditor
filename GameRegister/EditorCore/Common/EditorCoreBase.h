#ifndef _EDITOR_CORE_BASE_H_
#define _EDITOR_CORE_BASE_H_

class EditorCore;
class CTEventSystem;
class txCommandSystem;
class txSystemInfo;
class EditorCoreBase
{
public:
	static void notifyConstructDone();
protected:
	static EditorCore* mEditorCore;
	static CTEventSystem* mEventSystem;
	static txCommandSystem* mCommandSystem;
	static txSystemInfo* mSystemInfo;
};

#endif