#ifndef _TX_COMMAND_SYSTEM_H_
#define _TX_COMMAND_SYSTEM_H_

#include "CommonDefine.h"
#include "ThreadLock.h"

class txCommand;
class txCommandReceiver;
class txCommandSystem
{
public:
	txCommandSystem()
		:
		mShowDebugInfo(true)
	{}
	virtual ~txCommandSystem(){ destroy(); }
	virtual void init(bool showDebug = true){ mShowDebugInfo = showDebug; }
	void setShowDebugInfo(bool show) { mShowDebugInfo = show; }
	bool getShowDebugInfo() { return mShowDebugInfo; }
	void update(float elapsedTime);
	// 中断延时命令,在命令还未执行时,将命令销毁
	bool interruptCommand(txCommand* cmd);
	void pushCommand(txCommand* cmd, txCommandReceiver* cmdReceiver);
	// delayExecute是命令延时执行的时间,默认为0,只有new出来的命令才能延时执行
	// 子线程中发出的命令必须是延时执行的命令!
	void pushDelayCommand(txCommand* cmd, txCommandReceiver* cmdReceiver, float delayExecute = 0.01f);
	virtual void notifyReceiverDestroied(txCommandReceiver* receiver);
protected:
	void destroy();
protected:
	txVector<DelayCommand> mCommandBufferProcess;	// 用于处理的命令列表
	txVector<DelayCommand> mCommandBufferInput;		// 用于放入命令的命令列表
	bool mShowDebugInfo;
	ThreadLock mBufferLock;
};

#endif