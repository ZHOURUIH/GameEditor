#ifndef _DELAY_COMMAND_H_
#define _DELAY_COMMAND_H_

#include "Utility.h"

class CommandReceiver;
class GameCommand;
class DelayCommand : public Utility
{
public:
	void init(CommandReceiver* receiver, GameCommand* cmd, const float delayTime)
	{
		mReceiver = receiver;
		mCommand = cmd;
		mDelayTime = delayTime;
	}
public:
	CommandReceiver* mReceiver = nullptr;	// 命令接收者
	GameCommand* mCommand = nullptr;		// 命令对象
	float mDelayTime = 0.0f;				// 延迟时间
};

#endif