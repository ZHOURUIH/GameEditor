#ifndef _CMD_GLOBAL_DELAY_CALL_0_H_
#define _CMD_GLOBAL_DELAY_CALL_0_H_

#include "GameCommand.h"

// 用于延迟执行一个函数
class CmdGlobalDelayCall0 : public GameCommand
{
public:
	void reset() override
	{
		mFunction = nullptr;
	}
	void execute() override;
public:
	DelayCallback0 mFunction;
};

#endif