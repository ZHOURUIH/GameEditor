#include "Utility.h"
#include "txCommandSystem.h"
#include "txCommandReceiver.h"
#include "txCommand.h"
#include "ToolCoreLog.h"
#include "ToolCoreUtility.h"
#include "txMemoryTrace.h"

void txCommandSystem::update(float elapsedTime)
{
	// 同步命令输入列表到命令处理列表中
	LOCK(mBufferLock);
	int inputCount = mCommandBufferInput.size();
	for (int i = 0; i < inputCount; ++i)
	{
		mCommandBufferProcess.push_back(mCommandBufferInput[i]);
	}
	mCommandBufferInput.clear();
	UNLOCK(mBufferLock);

	// 如果一帧时间大于1秒,则认为是无效更新
	if (elapsedTime >= 1.0f)
	{
		return;
	}
	// 开始处理命令处理列表
	auto iter = mCommandBufferProcess.begin();
	for (; iter != mCommandBufferProcess.end();)
	{
		iter->mDelayTime -= elapsedTime;
		if (iter->mDelayTime <= 0.0f)
		{
			// 命令的延迟执行时间到了,则执行命令
			pushCommand(iter->mCommand, iter->mReceiver);
			TRACE_DELETE(iter->mCommand);
			iter = mCommandBufferProcess.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

bool txCommandSystem::interruptCommand(txCommand* cmd)
{
	if (cmd == NULL || !cmd->isDelayCommand())
	{
		return false;
	}
	auto iter = mCommandBufferProcess.begin();
	auto iterEnd = mCommandBufferProcess.end();
	for (; iter != iterEnd; ++iter)
	{
		// 找到该命令,然后销毁该命令,从列表中移除
		if (iter->mCommand == cmd)
		{
			LOGI("CommandSystem : interrupt 0x%p, %s, receiver : %s, file : %s, line : %d",
				cmd, cmd->showDebugInfo().c_str(), iter->mReceiver->getName().c_str(), StringUtility::getFileName(cmd->getFile()).c_str(), cmd->getLine());
			TRACE_DELETE(iter->mCommand);
			mCommandBufferProcess.erase(iter);
			return true;
		}
	}
	return false;
}

void txCommandSystem::pushCommand(txCommand* cmd, txCommandReceiver* cmdReceiver)
{
	if (cmd == NULL || cmdReceiver == NULL)
	{
		return;
	}
	cmd->setReceiver(cmdReceiver);
	if (mShowDebugInfo && cmd->getShowDebugInfo())
	{
		LOGI("CommandSystem : 0x%p, %s, receiver : %s, file : %s, line : %d",
			cmd, cmd->showDebugInfo().c_str(), cmdReceiver->getName().c_str(), StringUtility::getFileName(cmd->getFile()).c_str(), cmd->getLine());
	}
	cmdReceiver->receiveCommand(cmd);
}

void txCommandSystem::pushDelayCommand(txCommand* cmd, txCommandReceiver* cmdReceiver, float delayExecute)
{
	if (cmd == NULL || cmdReceiver == NULL)
	{
		return;
	}
	if (delayExecute <= 0.0f)
	{
		delayExecute = 0.01f;
	}
	if (mShowDebugInfo && cmd->getShowDebugInfo())
	{
		LOGI("CommandSystem : delay cmd : %f, info : 0x%p, %s, receiver : %s, file : %s, line : %d",
			delayExecute, cmd, cmd->showDebugInfo().c_str(), cmdReceiver->getName().c_str(), StringUtility::getFileName(cmd->getFile()).c_str(), cmd->getLine());
	}
	if (cmd->isDelayCommand())
	{
		DelayCommand delayCommand(delayExecute, cmd, cmdReceiver);
		LOCK(mBufferLock);
		mCommandBufferInput.push_back(delayCommand);
		UNLOCK(mBufferLock);
	}
	else
	{
		TOOL_CORE_ERROR("error : cmd is not a delay command, please use Command::createDelayCommand to create a delay command! \n\
command type : %s, file : %s, line : %d", typeid(*cmd).name(), cmd->getFile(), cmd->getLine());
	}
}

void txCommandSystem::destroy()
{
	int inputSize = mCommandBufferInput.size();
	for (int i = 0; i < inputSize; ++i)
	{
		TRACE_DELETE(mCommandBufferInput[i].mCommand);
	}
	mCommandBufferInput.clear();

	int processSize = mCommandBufferProcess.size();
	for (int i = 0; i < processSize; ++i)
	{
		TRACE_DELETE(mCommandBufferProcess[i].mCommand);
	}
	mCommandBufferProcess.clear();
}

void txCommandSystem::notifyReceiverDestroied(txCommandReceiver* receiver)
{
	LOCK(mBufferLock);
	auto iterCommandInput = mCommandBufferInput.begin();
	for (; iterCommandInput != mCommandBufferInput.end();)
	{
		if (iterCommandInput->mReceiver == receiver)
		{
			TRACE_DELETE(iterCommandInput->mCommand);
			iterCommandInput = mCommandBufferInput.erase(iterCommandInput);
		}
		else
		{
			++iterCommandInput;
		}
	}
	UNLOCK(mBufferLock);

	auto iterCommand = mCommandBufferProcess.begin();
	for (; iterCommand != mCommandBufferProcess.end();)
	{
		if (iterCommand->mReceiver == receiver)
		{
			TRACE_DELETE(iterCommand->mCommand);
			iterCommand = mCommandBufferProcess.erase(iterCommand);
		}
		else
		{
			++iterCommand;
		}
	}
}