#ifndef _COMMAND_SYSTEM_H_
#define _COMMAND_SYSTEM_H_

#include "ThreadLock.h"
#include "GameCommand.h"
#include "Map.h"
#include "Vector.h"

class DelayCommand;
class CommandSystem : public Utility
{
public:
	static CommandSystem* get()
	{
		return mInstance;
	}
	CommandSystem()
	{
		mInstance = this;
	}
	void update(const float elapsedTime);
	static DelayCommand* createDelayCommand(CommandReceiver* receiver, GameCommand* cmd, const float delayTime);
	static void destroyDelayCommand(DelayCommand* delayCommand);
	// 设置为静态函数,这样即使命令系统销毁了,仍然可以使用该函数销毁命令对象
	static void destroyCmd(GameCommand*& cmd);
	// 在任意线程中发送立即执行的命令,则该命令将在该线程中执行
	void pushCommand(GameCommand* cmd, CommandReceiver* cmdReceiver) const;
	// 仅限于执行已经被压入的命令,新创建的命令则需要使用pushCommand或pushCommandDelay等以push开头的函数进行发送
	void executeCommand(GameCommand* cmd, CommandReceiver* cmdReceiver) const;
	// 延迟执行的命令都会在主线程中执行
	// delayExecute是命令延时执行的时间,默认为0.001
	void pushCommandDelay(GameCommand* cmd, CommandReceiver* cmdReceiver, float delayExecute = 0.001f);
protected:
	Vector<DelayCommand*> mDelayCommandList;		// 延迟命令列表
	Vector<DelayCommand*> mExecuteList;				// 即将在这一帧执行的命令,该列表只在主线程进行写入,其他线程都是读取,所以暂不添加锁
	ThreadLock mDelayCommandListLock;
	static CommandSystem* mInstance;
};

#endif