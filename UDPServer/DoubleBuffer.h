#ifndef _DOUBLE_BUFFER_H_
#define _DOUBLE_BUFFER_H_

#include "Utility.h"
#include "ThreadLock.h"
#include "Vector.h"

// 双缓冲,线程安全的缓冲区,可在一个线程中写入数据,另一个线程中读取数据
// 存储的是T类型的列表
template<typename T>
class DoubleBuffer : public Utility
{
public:
	DoubleBuffer():
		mCloseBuffer(false),
		mReading(false)
	{}
	// 切换缓冲区,获得可读列表,在遍历可读列表期间,不能再次调用get,否则会出现不可预知的错误,并且该函数不能在多个线程中调用
	// 可读列表不再访问时需要调用endRead
	Vector<T>* get()
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
		Vector<T>* readList = nullptr;
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
			ERROR("不能在不同线程中获得可读列表,当前操作线程:" + uintToString(curThreadID) + ",首次操作线程:" + uintToString(mReadThreadID));
			readList = nullptr;
		}
		return readList;
	}
	void endRead() { mReading = false; }
	void add(const Vector<T>& value)
	{
		if (mCloseBuffer)
		{
			ERROR("缓冲区已经关闭,无法再写入数据");
			return;
		}

		const int curThreadID = getThreadID();
		Vector<T>* writeList = nullptr;
		LOCK(mBufferLock);
		if (mWriteThreadID == 0)
		{
			mWriteThreadID = curThreadID;
		}
		writeList = &(mBufferList[mWriteIndex]);
		UNLOCK(mBufferLock);
		if (mWriteThreadID != curThreadID)
		{
			ERROR("不能在不同线程中向双缓冲添加数据,当前操作线程:" + uintToString(curThreadID) + ",首次操作线程:" + uintToString(mWriteThreadID));
			return;
		}
		if (mWriteListLimit == 0 || writeList->size() + value.size() <= mWriteListLimit)
		{
			writeList->merge(value);
		}
	}
	// 向可写列表中添加数据
	void add(const T& value)
	{
		const int curThreadID = getThreadID();
		Vector<T>* writeList = nullptr;
		LOCK(mBufferLock);
		if (mWriteThreadID == 0)
		{
			mWriteThreadID = curThreadID;
		}
		writeList = &(mBufferList[mWriteIndex]);
		UNLOCK(mBufferLock);
		if (mWriteThreadID != curThreadID)
		{
			ERROR("不能在不同线程中向双缓冲添加数据");
			return;
		}
		if (mWriteListLimit == 0 || writeList->size() < mWriteListLimit)
		{
			writeList->push_back(value);
		}
	}
	void setWriteListLimit(const int limit) { mWriteListLimit = limit; }
	void setCloseBuffer(const bool close) { mCloseBuffer = close; }
	int getAllDataCount() const { return mBufferList[0].size() + mBufferList[1].size(); }
	void clear()
	{
		LOCK(mBufferLock);
		mBufferList[mReadIndex].clear();
		mBufferList[mWriteIndex].clear();
		mWriteThreadID = 0;
		mReadThreadID = 0;
		UNLOCK(mBufferLock);
	}
protected:
	Vector<T> mBufferList[2];			// 实际存储数据的缓冲区列表
	ThreadLock mReadingLock;			// ReadingStack的锁
	ThreadLock mBufferLock;				// mBufferList的锁
	string mReadingStack;				// 开始reading的堆栈信息
	int mWriteListLimit = 0;			// 写缓冲区可存储的最大数量,当到达上限时无法再写入,等于0表示无上限
	int mWriteThreadID = 0;				// 当前正在写缓冲区的线程ID
	int mReadThreadID = 0;				// 当前正在读缓冲区的线程ID
	int mWriteIndex = 0;				// 写缓冲区下标
	int mReadIndex = 1;					// 读缓冲区下标
	volatile atomic<bool> mCloseBuffer;	// 是否已经关闭buffer,关闭后将不能往buffer中写入数据
	volatile atomic<bool> mReading;		// 是否正在操作读缓冲区
};

#endif