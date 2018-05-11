#include "txCommandReceiver.h"
#include "txCommand.h"
#include "txCommandSystem.h"
#include "txEngineRoot.h"

txCommandReceiver::txCommandReceiver(const std::string& name)
:
mName(name)
{}

txCommandReceiver::~txCommandReceiver()
{
	// 通知命令系统有一个命令接受者已经被销毁了,需要取消命令缓冲区中的即将发给该接受者的命令
	// 此时需要确认引擎和命令系统没有被销毁
	if (txEngineRoot::getSingletonPtr() != NULL && txEngineRoot::getSingletonPtr()->getCommandSystem() != NULL)
	{
		txEngineRoot::getSingletonPtr()->getCommandSystem()->notifyReceiverDestroied(this);
	}
}

void txCommandReceiver::receiveCommand(txCommand* cmd)
{
	cmd->runStartCallBack();
	cmd->setExecuteState(ES_EXECUTING);
	cmd->execute();
	cmd->setExecuteState(ES_EXECUTED);
	cmd->runEndCallBack();
}