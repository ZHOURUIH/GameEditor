#ifndef _GAME_COMMAND_H_
#define _GAME_COMMAND_H_

#include "Utility.h"

class CommandReceiver;
class GameCommand : public Utility
{
public:
	// 命令执行
	virtual void execute() = 0;
	// 当命令压入命令系统时调用,无论是立即命令还是延迟命令或者是子线程命令
	virtual void onPush() {}
	virtual void onInterrupted() {}
	// 调试信息，由CommandSystem调用
	virtual void debugInfo(Array<1024>& buffer)		{ buffer.copy(mCmdType); }
	bool isDelayCommand() const						{ return mDelayCommand; }
	bool isThreadCommand() const					{ return mThreadCommand; }
	CommandReceiver* getReceiver() const			{ return mReceiver; }
	const char* getType() const						{ return mCmdType; }
	void setDelayCommand(const bool delay)			{ mDelayCommand = delay; }
	void setThreadCommand(const bool threadCommand)	{ mThreadCommand = threadCommand; }
	void setReceiver(CommandReceiver* Reciver)		{ mReceiver = Reciver; }
	void setType(const char* type)					{ mCmdType = type; }
protected:
	CommandReceiver* mReceiver = nullptr;				// 命令接受者
	const char* mCmdType = nullptr;						// 命令类型
	bool mThreadCommand = false;						// 是否在子线程执行的命令
	bool mDelayCommand = false;							// 是否是延迟执行的命令
};

#endif