#ifndef _THREAD_MANAGER_H_
#define _THREAD_MANAGER_H_

#include "Utility.h"
#include "CommonDefine.h"
#include "Map.h"

class CustomThread;
class ThreadManager : public Utility
{
public:
	static ThreadManager* get() 
	{
		if (mInstance == nullptr)
		{
			mInstance = new ThreadManager();
		}
		return mInstance;
	}
	virtual ~ThreadManager();
	CustomThread* createThread(const string& name, CustomThreadCallback callback, void* args, CustomThreadCallback preCmdCallback = nullptr, CustomThreadCallback endCmdCallback = nullptr);
	CustomThread* getThread(const MY_THREAD handle) const { return mThreadList.get(handle, nullptr); }
	void destroyThread(CustomThread*& thread);
protected:
	Map<MY_THREAD, CustomThread*> mThreadList;
	static ThreadManager* mInstance;
};

#endif