#ifndef _COMMAND_SYSTEM_H_
#define _COMMAND_SYSTEM_H_

#include "CommonDefine.h"

class Command;
class CommandReceiver;
class CommandSystem
{
public:
	CommandSystem()
		:
		mLockBuffer(false),
		mShowDebugInfo(true)
	{}
	virtual ~CommandSystem(){ destroy(); }
	virtual void init(bool showDebug = true){ mShowDebugInfo = showDebug; }
	void setShowDebugInfo(bool show) { mShowDebugInfo = show; }
	bool getShowDebugInfo() { return mShowDebugInfo; }
	void update(float elapsedTime);
	// 中断延时命令,在命令还未执行时,将命令销毁
	bool interruptCommand(Command* cmd);
	void pushCommand(Command* cmd, CommandReceiver* cmdReceiver);
	// delayExecute是命令延时执行的时间,默认为0,只有new出来的命令才能延时执行
	// 子线程中发出的命令必须是延时执行的命令!
	void pushDelayCommand(Command* cmd, CommandReceiver* cmdReceiver, float delayExecute = 0.01f);
	virtual void notifyReceiverDestroied(CommandReceiver* receiver);
protected:
	void destroy();
	void waitUnlockBuffer();
	void lockBuffer() { mLockBuffer = true; }
	void unlockBuffer() { mLockBuffer = false; }
protected:
	txVector<DelayCommand> mCommandBufferProcess;	// 用于处理的命令列表
	txVector<DelayCommand> mCommandBufferInput;		// 用于放入命令的命令列表
	bool mLockBuffer;
	bool mShowDebugInfo;
};

#endif