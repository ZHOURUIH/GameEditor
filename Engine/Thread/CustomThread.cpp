#include "CustomThread.h"
#include "Utility.h"
#include "txMemoryTrace.h"
#include "TimeLock.h"

CustomThread::CustomThread(const std::string& name)
{
	mName = name;
	mFinish = true;
	mRunning = false;
	mCallback = NULL;
	mThread = NULL;
	mTimeLock = NULL;
	mPause = false;
	mIsBackground = true;
}

void CustomThread::destroy()
{
	stop();
}

void CustomThread::start(CustomThreadCallback callback, void* args, const int& frameTimeMS)
{
	ENGINE_INFO("准备启动线程 : %s", mName.c_str());
	if (mThread != NULL)
	{
		return;
	}
	mTimeLock = TRACE_NEW(TimeLock, mTimeLock, frameTimeMS);
	mTimeLock->setForceSleep(5);
	mRunning = true;
	mCallback = callback;
	mArgs = args;
	CREATE_THREAD(mThread, run, this);
	ENGINE_INFO("线程启动成功 : %s", mName.c_str());
}

void CustomThread::stop()
{
	ENGINE_INFO("准备退出线程 : %s", mName.c_str());
	mRunning = false;
	mPause = false;
	while (!mIsBackground && !mFinish) {}
	CLOSE_THREAD(mThread);
	mCallback = NULL;
	TRACE_DELETE(mTimeLock);
	ENGINE_INFO("线程退出完成! 线程名 : %s", mName.c_str());
}

void CustomThread::updateThread()
{
	mFinish = false;
	while (mRunning)
	{
		mTimeLock->update();
		try
		{
			if (mPause)
			{
				continue;
			}
			if (!mCallback(mArgs))
			{
				break;
			}
		}
		catch (...)
		{
			ENGINE_INFO("捕获线程异常! 线程名 : %s", mName.c_str());
		}
	}
	mFinish = true;
}