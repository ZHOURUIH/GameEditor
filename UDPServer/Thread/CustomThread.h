#ifndef _CUSTOM_THREAD_H_
#define _CUSTOM_THREAD_H_

#include "ThreadLock.h"
#include "Utility.h"
#include "CommonDefine.h"
#include "Map.h"

class TimeLock;
class CustomThread : public Utility
{
public:
	CustomThread(const string& name, CustomThreadCallback callback, void* args, CustomThreadCallback preCmdCallback = nullptr, CustomThreadCallback endCmdCallback = nullptr);
	virtual ~CustomThread() { destroy(); }
	void destroy();
	void setTime(const int frameTimeMS, const int forceSleep = 5) const;
	void setBackground(const bool background)	{ mIsBackground = background; }
	void setShowFPS(const bool showFPS)			{ mShowFPS = showFPS; }
	void pause(const bool isPause)				{ mPause = isPause; }
	void setRunning(const bool running)			{ mRunning = running; }
	void setCmdDebug(const bool cmdDebug)		{ mCmdDebug = cmdDebug; }
	MY_THREAD getThreadHandle() const			{ return mThread; }
	float getFrameTime() const					{ return mFrameTime; }
	void* getArgs() const						{ return mArgs; }
protected:
	THREAD_CALLBACK_DECLEAR(run)
	{
		static_cast<CustomThread*>(args)->updateThread();
		return 0;
	}
	// 再写一个函数,避免在静态函数中只能通过变量访问成员变量
	void updateThread();
protected:
	string mName;									// 线程名
	Map<const char*, int> mCmdTypeList;				// 用于记录每种命令有多少个
	CustomThreadCallback mCallback = nullptr;		// 线程逻辑函数回调
	void* mArgs = nullptr;							// 线程启动参数
	TimeLock* mTimeLock = nullptr;					// 线程锁帧
	MY_THREAD mThread;								// 为了兼容windows和linux,句柄不写为原子类型
	int mCurFrameCount = 0;							// 当前记录的帧数,用于计算帧率
	int mThreadID = 0;								// 线程ID
	int mFPS = 0;									// 当前的帧率
	int mPID = 0;									// linux下的pid
	float mShowFPSTime = 0.0f;						// 显示线程帧率的计时
	float mFrameTime = 0.0f;						// 这一帧所经过的时间,用于线程回调中获取更新时间用
	float mCurTime = 0.0f;							// 当前线程计时,用于计算线程帧率
	bool mIsBackground = true;						// 是否为后台线程,如果是后台线程,则在应用程序关闭时,子线程会自动强制关闭
	bool mShowFPS = true;							// 是否打印此线程的帧率
	bool mOnlyOneThreadPushCmd = true;				// 是否只有一个线程往此线程发送命令
	bool mCmdDebug = false;							// 是否开启此线程的命令调试
	volatile atomic<bool> mRunning;					// 是否正在运行
	volatile atomic<bool> mFinish;					// 是否完成结束运行
	volatile atomic<bool> mPause;					// 是否暂停运行
};

#endif