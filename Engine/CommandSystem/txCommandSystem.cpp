#include "txEngineLog.h"
#include "Utility.h"
#include "txEngineRoot.h"
#include "txCommandSystem.h"
#include "txCommandReceiver.h"
#include "txCommand.h"

void txCommandSystem::update(float elapsedTime)
{
	// 同步命令输入列表到命令处理列表中
	syncCommandBuffer();

	// 如果一帧时间大于1秒,则认为是无效更新
	if (elapsedTime >= 1.0f)
	{
		return;
	}
	// 执行之前需要先清空列表
	mExecuteList.clear();
	// 开始处理命令处理列表
	for (int i = 0; i < mCommandBufferProcess.size(); ++i)
	{
		mCommandBufferProcess[i]->mDelayTime -= elapsedTime;
		if (mCommandBufferProcess[i]->mDelayTime <= 0.0f)
		{
			// 命令的延迟执行时间到了,则执行命令
			mExecuteList.push_back(mCommandBufferProcess[i]);
			mCommandBufferProcess.erase(mCommandBufferProcess.begin() + i);
			--i;
		}
	}
	int executeCount = mExecuteList.size();
	for (int i = 0; i < executeCount; ++i)
	{
		mExecuteList[i]->mCommand->setDelayCommand(false);
		if (mExecuteList[i]->mReceiver != NULL)
		{
			pushCommand(mExecuteList[i]->mCommand, mExecuteList[i]->mReceiver);
		}
		TRACE_DELETE(mExecuteList[i]);
	}
	// 执行完后清空列表
	mExecuteList.clear();
}

bool txCommandSystem::interruptCommand(const int& assignID)
{
	// 如果命令系统已经销毁了,则不能再中断命令
	if (mSystemDestroy)
	{
		return true;
	}
	if (assignID < 0)
	{
		ENGINE_ERROR("assignID invalid! : %d", assignID);
		return false;
	}
	syncCommandBuffer();
	int count = mCommandBufferProcess.size();
	FOR_STL(mCommandBufferProcess, int i = 0; i < count; ++i)
	{
		DelayCommand* delayInfo = mCommandBufferProcess[i];
		if (delayInfo->mCommand->getAssignID() == assignID)
		{
			ENGINE_INFO("CommandSystem : interrupt command : %d, %s, receiver : %s", assignID, delayInfo->mCommand->showDebugInfo().c_str(), delayInfo->mReceiver->getName().c_str());
			// 销毁回收命令
			mCommandPool->destroyCmd(delayInfo->mCommand);
			mCommandBufferProcess.erase(mCommandBufferProcess.begin() + i);
			TRACE_DELETE(delayInfo);
			return true;
		}
	}
	// 在即将执行的列表中查找
	int count0 = mExecuteList.size();
	FOR_STL(mExecuteList, int i = 0; i < count0; ++i)
	{
		if (mExecuteList[i]->mCommand->getAssignID() == assignID)
		{
			ENGINE_ERROR("cmd is in execute list! can not interrupt!");
			break;
		}
	}
	ENGINE_ERROR("not find cmd with assignID! %d", assignID);
	return false;
}

void txCommandSystem::pushCommand(txCommand* cmd, txCommandReceiver* cmdReceiver)
{
	if (cmd == NULL)
	{
		std::string receiverName = (cmdReceiver != NULL) ? cmdReceiver->getName() : EMPTY_STRING;
		ENGINE_ERROR("cmd is null! receiver : %s", receiverName.c_str());
		return;
	}
	if (cmdReceiver == NULL)
	{
		std::string cmdName = (cmd != NULL) ? cmd->getType() : EMPTY_STRING;
		ENGINE_ERROR("receiver is null! cmd : %s", cmdName.c_str());
		return;
	}
	if (!cmd->isValid())
	{
		ENGINE_ERROR("cmd is invalid! make sure create cmd use CommandSystem.newCmd! pushCommand cmd type : %s, assign id : %d", cmd->getType(), cmd->getAssignID());
		return;
	}
	if (cmd->isDelayCommand())
	{
		ENGINE_ERROR("cmd is a delay cmd! can not use pushCommand! assign id : %d, info : %s", cmd->getAssignID(), cmd->showDebugInfo());
		return;
	}
	cmd->setReceiver(cmdReceiver);
	if (cmd->getShowDebugInfo())
	{
		ENGINE_INFO("CommandSystem : %d, %s, receiver : %s", cmd->getAssignID(), cmd->showDebugInfo(), cmdReceiver->getName());
	}
	cmdReceiver->receiveCommand(cmd);

	// 销毁回收命令
	mCommandPool->destroyCmd(cmd);
}

void txCommandSystem::pushDelayCommand(txCommand* cmd, txCommandReceiver* cmdReceiver, float delayExecute)
{
	if (cmd == NULL)
	{
		std::string receiverName = (cmdReceiver != NULL) ? cmdReceiver->getName() : EMPTY_STRING;
		ENGINE_ERROR("cmd is null! receiver : %s", receiverName.c_str());
		return;
	}
	if (cmdReceiver == NULL)
	{
		std::string cmdName = (cmd != NULL) ? cmd->getType() : EMPTY_STRING;
		ENGINE_ERROR("receiver is null! cmd : %s", cmdName.c_str());
		return;
	}
	if (!cmd->isValid())
	{
		ENGINE_ERROR("cmd is invalid! make sure create cmd use CommandSystem.newCmd! pushDelayCommand cmd type : %s, assign id : %d", cmd->getType().c_str(), cmd->getAssignID());
		return;
	}
	if (!cmd->isDelayCommand())
	{
		ENGINE_ERROR("cmd is not a delay command, Command : %d, info : %s", cmd->getAssignID(), cmd->showDebugInfo());
		return;
	}
	if (delayExecute < 0.0f)
	{
		delayExecute = 0.0f;
	}
	if (cmd->getShowDebugInfo())
	{
		ENGINE_INFO("CommandSystem : delay cmd : %d, %.2f, info : %s, receiver : %s", cmd->getAssignID(), delayExecute, cmd->showDebugInfo().c_str(), cmdReceiver->getName());
	}
	DelayCommand* delayCommand = TRACE_NEW(DelayCommand, delayCommand, delayExecute, cmd, cmdReceiver);

	LOCK(mBufferLock);
	mCommandBufferInput.push_back(delayCommand);
	UNLOCK(mBufferLock);
}

void txCommandSystem::destroy()
{
	syncCommandBuffer();
	int processSize = mCommandBufferProcess.size();
	FOR_STL(mCommandBufferProcess, int i = 0; i < processSize; ++i)
	{
		mCommandPool->destroyCmd(mCommandBufferProcess[i]->mCommand);
		TRACE_DELETE(mCommandBufferProcess[i]);
	}
	END_FOR_STL(mCommandBufferProcess);
	mCommandBufferProcess.clear();

	mCommandPool->destroy();
	mSystemDestroy = true;
}

void txCommandSystem::notifyReceiverDestroied(txCommandReceiver* receiver)
{
	if (mSystemDestroy)
	{
		return;
	}
	// 先将命令
	syncCommandBuffer();
	// 同步列表中
	for (int i = 0; i < mCommandBufferProcess.size(); ++i)
	{
		if (mCommandBufferProcess[i]->mReceiver == receiver)
		{
			mCommandPool->destroyCmd(mCommandBufferProcess[i]->mCommand);
			TRACE_DELETE(mCommandBufferProcess[i]);
			mCommandBufferProcess.erase(mCommandBufferProcess.begin() + i);
			--i;
		}
	}
	// 执行列表中
	int count = mExecuteList.size();
	for (int i = 0; i < count; ++i)
	{
		// 已执行或正在执行的命令不作判断,该列表无法删除元素,只能将接收者设置为null
		if (mExecuteList[i] != NULL && mExecuteList[i]->mReceiver == receiver && mExecuteList[i]->mCommand->getExecuteState() == ES_NOT_EXECUTE)
		{
			mExecuteList[i]->mReceiver = NULL;
		}
	}
}

void txCommandSystem::syncCommandBuffer()
{
	LOCK(mBufferLock);
	int inputCount = mCommandBufferInput.size();
	if (inputCount > 0)
	{
		for (int i = 0; i < inputCount; ++i)
		{
			mCommandBufferProcess.push_back(mCommandBufferInput[i]);
		}
		mCommandBufferInput.clear();
	}
	UNLOCK(mBufferLock);
}