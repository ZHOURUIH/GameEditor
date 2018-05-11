#ifndef _EDITOR_CORE_BASE_H_
#define _EDITOR_CORE_BASE_H_

class EditorCore;
class EventSystem;
class CommandSystem;
class DownloadManager;
class LibCurlDownloadManager;
class EditorCoreBase
{
public:
	static void notifyConstructDone();
protected:
	static EditorCore* mEditorCore;
	static EventSystem* mEventSystem;
	static CommandSystem* mCommandSystem;
	static DownloadManager* mDownloadManager;
	static LibCurlDownloadManager* mLibCurlDownloadManager;
};

#endif