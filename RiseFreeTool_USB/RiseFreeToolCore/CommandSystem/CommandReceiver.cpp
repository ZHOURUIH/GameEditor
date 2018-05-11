#include "CommandReceiver.h"
#include "Command.h"
#include "CommandSystem.h"
#include "RiseFreeToolCore.h"

CommandReceiver::CommandReceiver(std::string name)
:
mName(name)
{}

CommandReceiver::~CommandReceiver()
{
	// 通知命令系统有一个命令接受者已经被销毁了,需要取消命令缓冲区中的即将发给该接受者的命令
	// 此时需要确认引擎和命令系统没有被销毁
	if (RiseFreeToolCore::getSingletonPtr() != NULL && RiseFreeToolCore::getSingletonPtr()->getCommandSystem() != NULL)
	{
		RiseFreeToolCore::getSingletonPtr()->getCommandSystem()->notifyReceiverDestroied(this);
	}
}

void CommandReceiver::receiveCommand(Command *cmd)
{
	cmd->runStartCallBack();
	cmd->execute();
	cmd->runEndCallBack();
}