#ifndef _EDITOR_CORE_BASE_H_
#define _EDITOR_CORE_BASE_H_

class EditorCore;
class SocketNetManager;
class CTEventSystem;
class txCommandSystem;
class txSystemInfo;
class LibCurlNetManager;
class EditorCoreBase
{
public:
	static void notifyConstructDone();
protected:
	static EditorCore* mEditorCore;
	static SocketNetManager* mSocketNetManager;
	static CTEventSystem* mEventSystem;
	static txCommandSystem* mCommandSystem;
	static txSystemInfo* mSystemInfo;
	static LibCurlNetManager* mLibCurlNetManager;
};

#endif