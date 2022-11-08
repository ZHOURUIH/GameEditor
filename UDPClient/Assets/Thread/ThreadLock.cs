using System;
using System.Threading;
using UnityEngine;

public class ThreadLock
{
	protected volatile int mLockCount;	// 是否锁定
	protected string mFileName;         // 加锁的文件名
	protected int mLockThreadID;        // 当前获得锁的线程ID
	protected int mLine;                // 加锁的行号
	public bool isLocked() { return mLockCount == 1; }
	public void waitForUnlock()
	{
		if (mLockThreadID == Thread.CurrentThread.ManagedThreadId && isLocked())
		{
			Debug.LogError("线程死锁");
		}
		// 一直尝试将mLockCount设置为1,然后判断设置之前mLockCount是否为0
		// 如果mLockCount在这之前为0,则表示锁在其他线程被释放,当前线程成功获得锁
		while (Interlocked.Exchange(ref mLockCount, 1) != 0) { }
		mLockThreadID = Thread.CurrentThread.ManagedThreadId;
	}
	public void unlock()
	{
		mLockThreadID = 0;
		Interlocked.Exchange(ref mLockCount, 0);
	}
}