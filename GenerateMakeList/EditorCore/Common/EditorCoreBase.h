#ifndef _EDITOR_CORE_BASE_H_
#define _EDITOR_CORE_BASE_H_

class EditorCore;
class SocketNetManager;
class EventSystem;
class CommandSystem;
class LibCurlDownloadManager;
class EditorCoreBase
{
public:
	static void notifyConstructDone();
protected:
	static EditorCore* mEditorCore;
	//static SocketNetManager* mSocketNetManager;
	static EventSystem* mEventSystem;
	static CommandSystem* mCommandSystem;
	static LibCurlDownloadManager* mLibCurlDownloadManager;
};

#endif