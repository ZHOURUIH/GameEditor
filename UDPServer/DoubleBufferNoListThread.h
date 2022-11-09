#ifndef _DOUBLE_BUFFER_NO_LIST_THREAD_H_
#define _DOUBLE_BUFFER_NO_LIST_THREAD_H_

#include "Utility.h"
#include "ThreadLock.h"

// 双缓冲,线程安全的缓冲区,可在多个线程中写入数据,另一个线程中读取数据
// 存储的是T类型
template<typename T>
class DoubleBufferNoListThread : public Utility
{
public:
	DoubleBufferNoListThread():
		mCloseBuffer(false),
		mReading(false)
	{}
	// 切换缓冲区,获得可读列表,在遍历可读列表期间,不能再次调用get,否则会出现不可预知的错误,并且该函数不能在多个线程中调用
	// 当不再使用可读列表时,需要调用endRead
	T* startRead()
	{
		if (mReading)
		{
			ERROR("当前正在操作读缓冲区,不能再次获取读缓冲区, stack:" + mReadingStack);
			return nullptr;
		}
		LOCK(mReadingLock);
		mReading = true;
#ifdef WIN32
		//mReadingStack = getStackTrace(2);
#endif
		UNLOCK(mReadingLock);
		const int curThreadID = getThreadID();
		T* readList = nullptr;
		LOCK(mBufferLock);
		if (mReadThreadID == 0)
		{
			mReadThreadID = curThreadID;
		}
		swap(mReadIndex, mWriteIndex);
		readList = &(mBufferList[mReadIndex]);
		UNLOCK(mBufferLock);
		if (mReadThreadID != curThreadID)
		{
			ERROR("不能在不同线程中获得可读列表,当前操作线程:" + intToString(curThreadID) + ",首次操作线程:" + intToString(mReadThreadID));
			readList = nullptr;
		}
		return readList;
	}
	void endRead() { mReading = false; }
	// 向可写列表中添加数据
	T& startWrite()
	{
		if (mCloseBuffer)
		{
			ERROR("缓冲区已关闭,将不允许再写入数据");
		}
		mBufferLock.waitForUnlock(__FILE__, __LINE__);
		return mBufferList[mWriteIndex];
	}
	void endWrite()
	{
		mBufferLock.unlock();
	}
	void setCloseBuffer(bool close) { mCloseBuffer = close; }
protected:
	T mBufferList[2];					// 实际存储数据的缓冲区列表
	ThreadLock mReadingLock;			// ReadingStack的锁
	ThreadLock mBufferLock;				// mBufferList的锁
	string mReadingStack;				// 开始reading的堆栈信息
	int mReadThreadID = 0;				// 当前正在读缓冲区的线程ID
	int mWriteIndex = 0;				// 写缓冲区下标
	int mReadIndex = 1;					// 读缓冲区下标
	volatile atomic<bool> mCloseBuffer;	// 是否关闭buffer,关闭后将无法写入buffer
	volatile atomic<bool> mReading;		// 是否正在操作读缓冲区
};

#endif