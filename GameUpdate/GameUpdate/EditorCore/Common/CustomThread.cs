using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

// 返回值表示是否继续运行该线程
public delegate bool CustomThreadCallback();

public class CustomThread : EditorBase
{
	protected bool mRunning;
	protected bool mFinish;
	protected bool mPause;
	protected CustomThreadCallback mCallback;
	protected Thread mThread;
	protected ThreadTimeLock mTimeLock;
	protected string mName;
	protected bool mIsBackground;		// 是否为后台线程,如果是后台线程,则在应用程序关闭时,子线程会自动强制关闭
	public CustomThread(string name)
	{
		mName = name;
		mFinish = true;
		mRunning = false;
		mCallback = null;
		mThread = null;
		mTimeLock = null;
		mPause = false;
		mIsBackground = true;
	}
	public void destroy()
	{
		stop();
	}
	public void setBackground(bool background)
	{
		mIsBackground = background;
		if(mThread != null)
		{
			mThread.IsBackground = mIsBackground;
		}
	}
	public void start(CustomThreadCallback callback, int frameTimeMS = 15)
	{
		logConsole("准备启动线程 : " + mName);
		if (mThread != null)
		{
			return;
		}
		mTimeLock = new ThreadTimeLock(frameTimeMS);
		mRunning = true;
		mCallback = callback;
		mThread = new Thread(run);
		mThread.Name = mName;
		mThread.Start();
		mThread.IsBackground = mIsBackground;
		logConsole("线程启动成功 : " + mName);
	}
	public void pause(bool isPause)
	{
		mPause = isPause;
	}
	public void stop()
	{
		logConsole("准备退出线程 : " + mName);
		mRunning = false;
		while (!mIsBackground && !mFinish) { }
		if (mThread != null && !mIsBackground)
		{
			mThread.Abort();
		}
		mThread = null;
		mCallback = null;
		mTimeLock = null;
		mPause = false;
		logConsole("线程退出完成! 线程名 : " + mName);
	}
	protected void run()
	{
		mFinish = false;
		while (mRunning)
		{
			mTimeLock.update();
			try
			{
				if(mPause)
				{
					continue;
				}
				if(!mCallback())
				{
					break;
				}
			}
			catch (Exception e)
			{
				logConsole("捕获线程异常! 线程名 : " + mName + ", " + e.Message + ", " + e.StackTrace);
			}
		}
		mFinish = true;
	}
}