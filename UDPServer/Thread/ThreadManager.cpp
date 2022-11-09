#include "ThreadManager.h"
#include "CustomThread.h"

ThreadManager* ThreadManager::mInstance;

ThreadManager::~ThreadManager()
{
	if (mThreadList.size() > 0)
	{
		LOG(("有" + intToString(mThreadList.size()) + "个线程未正常关闭").c_str());
	}
	// 强制关闭所有线程
	for(const auto& iter : mThreadList)
	{
		delete iter.second;
	}
	mThreadList.clear();
}

CustomThread* ThreadManager::createThread(const string& name, CustomThreadCallback callback, void* args, CustomThreadCallback preCmdCallback, CustomThreadCallback endCmdCallback)
{
	CustomThread* thread = new CustomThread(name, callback, args, preCmdCallback, endCmdCallback);
	mThreadList.insert(thread->getThreadHandle(), thread);
	return thread;
}

void ThreadManager::destroyThread(CustomThread*& thread)
{
	if (thread == nullptr)
	{
		return;
	}
	mThreadList.erase(thread->getThreadHandle());
	delete thread;
}