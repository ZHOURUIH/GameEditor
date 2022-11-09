#include "CustomThread.h"
#include "TimeLock.h"

CustomThread::CustomThread(const string& name, CustomThreadCallback callback, void* args, CustomThreadCallback preCmdCallback, CustomThreadCallback endCmdCallback) :
	mName(name),
	mCallback(callback),
	mArgs(args),
	mRunning(true),
	mFinish(true),
	mPause(false)
{
	mTimeLock = new TimeLock(15, 5);
	CREATE_THREAD(mThread, run, this);
	LOG("线程启动成功:" + mName);
}

void CustomThread::destroy()
{
	// 退出线程
	mRunning = false;
	mPause = false;
	while (!mIsBackground && !mFinish) {}
	CLOSE_THREAD(mThread);
	mCallback = nullptr;
	delete mTimeLock;
	LOG("线程退出完成! 线程名:" + mName);
}

void CustomThread::setTime(const int frameTimeMS, const int forceSleep) const
{
	mTimeLock->setFrameTime(frameTimeMS);
	mTimeLock->setForceSleepTime(forceSleep);
}

void CustomThread::updateThread()
{
	if (mThreadID == 0)
	{
		mThreadID = getThreadID();
		mPID = 0;
		LOG("线程ID:" + intToString(mThreadID) + ", PID:" + intToString(mPID) + ", 线程名:" + mName);
	}
	mFinish = false;
	while (mRunning)
	{
		if (mTimeLock == nullptr)
		{
			break;
		}
		mFrameTime = mTimeLock->update() * 0.001f;
		if (mShowFPS)
		{
			mCurTime += mFrameTime;
			++mCurFrameCount;
			if (mCurTime >= 1.0f)
			{
				mFPS = mCurFrameCount;
				mCurFrameCount = 0;
				mCurTime -= 1.0f;
			}
			mShowFPSTime += mFrameTime;
			// 每10秒记录一次线程帧率
			if (mShowFPSTime >= 10.0f)
			{
				mShowFPSTime -= 10.0f;
			}
		}
		try
		{
			if (mPause)
			{
				continue;
			}

			if (mCallback != nullptr)
			{
				mCallback(this);
			}
		}
		catch (const exception& e)
		{
			LOG(string("捕获线程异常:") + e.what() + ", 线程名:" + mName);
		}
	}
	mFinish = true;
}