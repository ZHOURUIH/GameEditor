using UnityEngine;
using System.Collections.Generic;
using System;
using static MathUtility;

// 命令系统,用于处理所有命令相关逻辑
public class CommandSystem
{
	protected List<Command> mCommandBufferProcess;	// 用于处理的命令列表
	protected List<Command> mCommandBufferInput;	// 用于放入命令的命令列表,收集一帧中各个线程的命令
	protected List<Command> mExecuteList;			// 即将在这一帧执行的命令
	protected ThreadLock mBufferLock;				// mCommandBufferInput的线程锁
	public CommandSystem()
	{
		mCommandBufferProcess = new List<Command>();
		mCommandBufferInput = new List<Command>();
		mExecuteList = new List<Command>();
		mBufferLock = new ThreadLock();
	}
	public void destroy()
	{
		mCommandBufferInput.Clear();
		mCommandBufferProcess.Clear();
	}
	public void update(float elapsedTime)
	{
		// 同步命令输入列表到命令处理列表中
		syncCommandBuffer();
		// 执行之前需要先清空列表
		mExecuteList.Clear();
		// 开始处理命令处理列表
		int count = mCommandBufferProcess.Count;
		for (int i = 0; i < count; ++i)
		{
			Command cmd = mCommandBufferProcess[i];
			cmd.setDelayTime(cmd.getDelayTime() - elapsedTime);
			if (cmd.getDelayTime() <= 0.0f)
			{
				// 命令的延迟执行时间到了,则执行命令
				mExecuteList.Add(cmd);
				mCommandBufferProcess.RemoveAt(i);
				--i;
				--count;
			}
		}
		int executeCount = mExecuteList.Count;
		for (int i = 0; i < executeCount; ++i)
		{
			Command cmd = mExecuteList[i];
			cmd.setDelayCommand(false);
			CommandReceiver receiver = cmd.getReceiver();
			if (receiver != null)
			{
				receiver.removeReceiveDelayCmd();
				pushCommand(cmd, receiver);
			}
		}
		// 执行完后清空列表
		mExecuteList.Clear();
	}
	public void interruptCommand(List<long> assignIDList, bool showError = true)
	{
		int count = assignIDList.Count;
		for (int i = 0; i < count; ++i)
		{
			interruptCommand(assignIDList[i], showError);
		}
	}
	// 中断命令
	public bool interruptCommand(long assignID, bool showError = true)
	{
		if (assignID < 0)
		{
			return false;
		}

		syncCommandBuffer();
		int count = mCommandBufferProcess.Count;
		for (int i = 0; i < count; ++i)
		{
			Command cmd = mCommandBufferProcess[i];
			if (cmd.getID() == assignID)
			{
				mCommandBufferProcess.Remove(cmd);
				return true;
			}
		}

		bool success = false;
		// 在即将执行的列表中查找,不能删除列表元素，只能将接收者设置为空来阻止命令执行,如果正在执行该命令,则没有效果
		int executeCount = mExecuteList.Count;
		for (int i = 0; i < executeCount; ++i)
		{
			Command cmd = mExecuteList[i];
			// 为了确保一定不会出现在命令执行过程中中断了当前正在执行的命令,以及已经执行过的命令
			if (cmd.getID() == assignID && cmd.getReceiver() != null && cmd.isDelayCommand())
			{
				cmd.getReceiver().removeReceiveDelayCmd();
				cmd.setReceiver(null);
				success = true;
				break;
			}
		}
		return success;
	}
	// 在当前线程中执行命令
	public void pushCommand(Command cmd, CommandReceiver cmdReceiver)
	{
		cmd.setReceiver(cmdReceiver);
		cmd.execute();
	}
	// 延迟执行命令,会延迟到主线程执行
	public void pushDelayCommand(Command cmd, CommandReceiver cmdReceiver, float delayExecute)
	{
		cmdReceiver.addReceiveDelayCmd();
		if (delayExecute < 0.0f)
		{
			delayExecute = 0.0f;
		}
		cmd.setDelayTime(delayExecute);
		cmd.setReceiver(cmdReceiver);
		mBufferLock.waitForUnlock();
		mCommandBufferInput.Add(cmd);
		mBufferLock.unlock();
	}
	//------------------------------------------------------------------------------------------------------------------------------
	protected void syncCommandBuffer()
	{
		mBufferLock.waitForUnlock();
		mCommandBufferProcess.AddRange(mCommandBufferInput);
		mCommandBufferInput.Clear();
		mBufferLock.unlock();
	}
}