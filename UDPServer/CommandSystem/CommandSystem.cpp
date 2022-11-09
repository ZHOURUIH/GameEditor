#include "CommandSystem.h"
#include "DelayCommand.h"

CommandSystem* CommandSystem::mInstance;

DelayCommand* CommandSystem::createDelayCommand(CommandReceiver* receiver, GameCommand* cmd, const float delayTime)
{
	DelayCommand* delayCommand = new DelayCommand();
	delayCommand->init(receiver, cmd, delayTime);
	return delayCommand;
}

void CommandSystem::destroyDelayCommand(DelayCommand* delayCommand)
{
	delete delayCommand;
}

void CommandSystem::destroyCmd(GameCommand*& cmd)
{
	delete cmd;
}

void CommandSystem::update(const float elapsedTime)
{
	// 执行之前需要先清空列表
	mExecuteList.clear();
	// 开始处理命令处理列表
	LOCK(mDelayCommandListLock);
	for(int i = 0; i < mDelayCommandList.size(); ++i)
	{
		DelayCommand*& delayCmd = mDelayCommandList[i];
		if (delayCmd == nullptr)
		{
			continue;
		}
		delayCmd->mDelayTime -= elapsedTime;
		if (delayCmd->mDelayTime <= 0.0f)
		{
			// 命令的延迟执行时间到了,则执行命令
			mExecuteList.push_back(delayCmd);
			mDelayCommandList.eraseAt(i--, 1, false);
		}
	}
	UNLOCK(mDelayCommandListLock);

	// 执行所有需要执行的命令
	for(auto item : mExecuteList)
	{
		DelayCommand*& delayCmd = item;
		if (delayCmd == nullptr)
		{
			continue;
		}
		GameCommand*& cmd = delayCmd->mCommand;
		cmd->setDelayCommand(false);
		if (delayCmd->mReceiver != nullptr)
		{
			executeCommand(cmd, delayCmd->mReceiver);
		}
		else
		{
			// 销毁回收命令
			destroyCmd(cmd);
		}
		destroyDelayCommand(delayCmd);
	}
	mExecuteList.clear();
}

void CommandSystem::pushCommand(GameCommand* cmd, CommandReceiver* cmdReceiver) const
{
	if (cmdReceiver == nullptr)
	{
		destroyCmd(cmd);
		return;
	}
	cmd->onPush();
	executeCommand(cmd, cmdReceiver);
}

void CommandSystem::pushCommandDelay(GameCommand* cmd, CommandReceiver* cmdReceiver, float delayExecute)
{
	cmd->onPush();
	clampMin(delayExecute);
	LOCK(mDelayCommandListLock);
	mDelayCommandList.push_back(createDelayCommand(cmdReceiver, cmd, delayExecute));
	UNLOCK(mDelayCommandListLock);
}

void CommandSystem::executeCommand(GameCommand* cmd, CommandReceiver* cmdReceiver) const
{
	cmd->setReceiver(cmdReceiver);
	cmd->execute();
	// 销毁回收命令
	destroyCmd(cmd);
}