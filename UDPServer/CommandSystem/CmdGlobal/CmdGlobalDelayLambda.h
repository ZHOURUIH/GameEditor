#ifndef _CMD_GLOBAL_DELAY_LAMBDA_H_
#define _CMD_GLOBAL_DELAY_LAMBDA_H_

#include "GameCommand.h"

// 用于延迟执行一个lambda表达式
class CmdGlobalDelayLambda : public GameCommand
{
public:
	void reset() override
	{
		mFunction = nullptr;
	}
	void execute() override;
public:
	DelayLambda mFunction;
};

#endif