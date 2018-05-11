#ifndef _CUSTOM_THREAD_H_
#define _CUSTOM_THREAD_H_

#include "txEngineDefine.h"
#include "txEngineLog.h"

class TimeLock;
class CustomThread
{
public:
	CustomThread(const std::string& name);
	void destroy();
	void setBackground(const bool& background){mIsBackground = background;}
	void start(CustomThreadCallback callback, void* args, const int& frameTimeMS = 15);
	void pause(const bool& isPause){mPause = isPause;}
	void stop();
protected:
	THREAD_CALLBACK_DECLEAR(run)
	{
		((CustomThread*)args)->updateThread();
		return NULL;
	}
	// 再写一个函数,避免在静态函数中只能通过变量访问成员变量
	void updateThread();
protected:
	volatile std::atomic<bool> mRunning;
	volatile std::atomic<bool> mFinish;
	volatile std::atomic<bool> mPause;
	volatile std::atomic<TX_THREAD> mThread;
	CustomThreadCallback mCallback;
	TimeLock* mTimeLock;
	std::string mName;
	bool mIsBackground;		// 是否为后台线程,如果是后台线程,则在应用程序关闭时,子线程会自动强制关闭
	void* mArgs;
};

#endif