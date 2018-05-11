#ifndef _TX_COMMAND_SYSTEM_H_
#define _TX_COMMAND_SYSTEM_H_

#include "txEngineDefine.h"
#include "ThreadLock.h"
#include "CommandPool.h"

class txCommand;
class txCommandReceiver;
class txCommandSystem
{
public:
	txCommandSystem()
	{
		mCommandPool = TRACE_NEW(CommandPool, mCommandPool);
		mSystemDestroy = false;
	}
	virtual void init()
	{ 
		mCommandPool->init(); 
	}
	virtual void update(float elapsedTime);
	// 创建命令
	template<typename T>
	T* newCmd(T*& cmd, const std::string& file, const int& line, const bool& show = true, const bool& delay = false)
	{
		cmd = mCommandPool->newCmd<T>(show, delay);
		cmd->setFile(file);
		cmd->setLine(line);
		return cmd;
	}
	// 中断延时命令,在命令还未执行时,将命令销毁
	bool interruptCommand(const int& assignID);
	// 在任意线程中发送立即执行的命令,则该命令将在该线程中执行
	void pushCommand(txCommand* cmd, txCommandReceiver* cmdReceiver);
	template<typename T>
	void pushCommand(txCommandReceiver* cmdReceiver, const bool& show = true)
	{
		T cmd = newCmd<T>(show, false);
		pushCommand(cmd, cmdReceiver);
	}
	// 延迟执行的命令都会在主线程中执行
	// delayExecute是命令延时执行的时间,默认为0.01,只有new出来的命令才能延时执行
	void pushDelayCommand(txCommand* cmd, txCommandReceiver* cmdReceiver, float delayExecute = 0.001f);
	template<typename T>
	void pushDelayCommand(txCommandReceiver* cmdReceiver, const float& delayExecute = 0.001f, const bool& show = true)
	{
		T cmd = newCmd<T>(show, true);
		pushDelayCommand(cmd, cmdReceiver, delayExecute);
	}
	virtual void notifyReceiverDestroied(txCommandReceiver* receiver);
protected:
	virtual void destroy();
	void syncCommandBuffer();
protected:
	CommandPool* mCommandPool;
	txVector<DelayCommand*> mCommandBufferProcess;	// 用于处理的命令列表
	txVector<DelayCommand*> mCommandBufferInput;	// 用于放入命令的命令列表
	txVector<DelayCommand*> mExecuteList;			// 即将在这一帧执行的命令
	ThreadLock mBufferLock;
	bool mSystemDestroy;	// 命令系统是否已经销毁
};

#endif