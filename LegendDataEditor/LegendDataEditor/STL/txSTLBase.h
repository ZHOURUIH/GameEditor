#ifndef _TX_STL_BASE_H_
#define _TX_STL_BASE_H_

enum STL_LOCK
{
	SL_NONE,
	SL_READ,
	SL_WRITE,
};

class txSTLBase
{
public:
	txSTLBase()
	{
		mLock = SL_NONE;
		mLine = 0;
		mReadLockCount = 0;
	}
	virtual ~txSTLBase(){}
	// 循环遍历列表之前必须锁定
	virtual void lock(STL_LOCK lockType, const std::string& file, int line)
	{
		// 当前为锁定写入,则不允许再加任何锁,或者当前为读锁定,则不能添加写锁定
		if (mLock == SL_WRITE || mLock == SL_READ && lockType != SL_READ || lockType == SL_NONE)
		{
			static char temp[512];
			SPRINTF(temp, 512, "error : can not lock stl before unlock! curlock file : %s, line : %d, lastlock file : %s, line : %d", file.c_str(), line, mFile.c_str(), mLine);
			directError(temp);
			return;
		}
		mFile = file;
		mLine = line;
		mLock = lockType;
		if (mLock == SL_READ)
		{
			++mReadLockCount;
		}
	}
	// 循环遍历列表结束以后必须解锁
	virtual void unlock(STL_LOCK lockType) 
	{
		// 锁定类型不一致,则不能解锁
		if (mLock == SL_NONE || mLock != lockType)
		{
			static char temp[512];
			SPRINTF(temp, 512, "error : can not unlock stl before lock! file : %s, line : %d", mFile.c_str(), mLine);
			directError(temp);
			return;
		}
		if (mLock == SL_READ)
		{
			--mReadLockCount;
			if (mReadLockCount == 0)
			{
				mLock = SL_NONE;
			}
		}
		else if(mLock == SL_WRITE)
		{
			mLock = SL_NONE;
		}
	}
protected:
	void checkLock()
	{
		if (mLock != SL_NONE)
		{
			directError("error : stl is locked! can not modify stl!");
		}
	}
	void directError(const std::string& info)
	{
		//LOG_ERROR("%s", info.c_str());
	}
protected:
	STL_LOCK mLock;
	int mReadLockCount;		// 读锁定的次数,读锁定可以叠加,计算读锁定的次数,当读锁定次数为0时,取消锁定
	std::string mFile;
	int mLine;
};

#endif